/*---
 * Title        : Project - 2b,
 * Course       : DBI
 * Team Members : Priyam Saikia, Ruchika Mishra
*/

#include "../include/SortedDBFile.h"
#include "../include/GenericDBFile.h"
#include "TwoWayList.h"
#include "Record.h"
#include "Schema.h"
#include "File.h"
#include "Comparison.h"
#include "ComparisonEngine.h"
#include "DBFile.h"
#include "Defs.h"
#include "../include/HeapDBFile.h"
#include "../include/SequentialProcessor.h"
#include <iostream>
#include <string.h>
#include <stdio.h>

using namespace std;

SortedDBFile::SortedDBFile(): GenericDBFile() {
	sortInfo = NULL;
	f_path = NULL;
	in = NULL;
	out = NULL;
	cursor = new Page();
	cursorIndex = 0;
	scanner = new SequentialProcessor(file, *cursor, cursorIndex);
	rwState = Reading;
	getNextState = NoCNF;
}

SortedDBFile::SortedDBFile(File &file, Input &rfile, Settings &config, ComparisonEngine &comp, const char *_f_path, SortInfo *_sortInfo):
GenericDBFile(file, rfile, config, comp), f_path(_f_path), sortInfo(_sortInfo), cursor(new Page()) {
	in = NULL;
	out = NULL;
	rwState = Reading;
	getNextState = NoCNF;
	cursorIndex = 0;
	scanner = new SequentialProcessor(file, *cursor, cursorIndex);
}

SortedDBFile::~SortedDBFile () {
	Reset();
	delete cursor;
	delete scanner;
	delete sortInfo;
}

void SortedDBFile::Load (Schema &f_schema, const char *loadpath) {
	Record r;
	Load(f_schema, loadpath, r);
}

void SortedDBFile::Load (Schema &f_schema, const char *loadpath, Record &record) {
	getNextState = NoCNF;
	rwState = Writing;
	if(!rfile.FileExists(loadpath)) throw std::runtime_error(loadpath + std::string(" could not be found."));
	// File exists
	FILE *file = fopen(loadpath, "r");
	while(record.SuckNextRecord(&f_schema, file)) {
		Add(record);
	}
	fclose(file);
}

void SortedDBFile::MoveFirst () {
	
	Flush(); 
	
	
	if(GetLength() > 0) file.GetPage(cursor, 0); 
	
	else cursor -> EmptyItOut(); 
	
	
	cursorIndex = 0;
	
	getNextState = NoCNF;
	
	rwState = Reading;
}

void SortedDBFile::Add (Record &rec) {
	in->Insert(&rec);

	rwState = Writing;
	getNextState = NoCNF;
}

int SortedDBFile::GetNext (Record &fetchme) {
	Flush();
	return scanner->GetNext(fetchme);
}

int SortedDBFile::GetNext (Record &fetchme, CNF &cnf, Record &literal) {
	Flush();

	
	if(getNextState == NoCNF) {
		getNextState = UseCNF;
		OrderMaker query;

		// If we can make a query but the binary search fails, then we could not find any valid
		// Record. Note that we should not fail just because we could not make a query.
		if(cnf.MakeQuery(*(sortInfo->myOrder), query) && !BinarySearch(literal, query)) return false;
	}
	while(this->GetNext(fetchme)) {
		if(comp.Compare(&fetchme, &literal, &cnf)) {
			return 1;
		}
	}

	return 0;
}

void SortedDBFile::Flush() {
	if(rwState == Writing) {
		File file;
		Flush(file);
		rwState = Reading;
	}
}

void SortedDBFile::Flush(File &temp) {
	char tempname[] = "tempXXXXXX";
	rfile.MakeTemp(tempname);
	string buf(f_path);
	Input temprfile;
	Settings tempconfig;
	ComparisonEngine tempcomp;

	// Here's where the real work gets done to merge the Records in bigq and the Records in file.
	temp.Open(0, tempname);
	HeapDBFile db = HeapDBFile(temp,temprfile, tempconfig, tempcomp);
	Flush(db);

	// Cleanup and re-assign files
	temp.Close();
	file.Close();
	if(rfile.Remove(buf.c_str()) != 0) throw runtime_error("SortedDBFile::Flush failed to remove a file");;
	if(rfile.Rename(tempname, buf.c_str()) != 0) throw runtime_error("SortedDBFile::Flush failed to rename a file");
	file.Open(1, buf.c_str());
}

void SortedDBFile::Flush(HeapDBFile &temp) {
	Pipe p = Pipe(PIPE_SIZE);
	PipedPage p1 = PipedPage(p, &file);
	PipedPage p2 = PipedPage(*out);
	pthread_t worker;
	Pipe sortedrecs;
	int runlen = 1;
	TPMMS tpmms = TPMMS(sortedrecs, sortedrecs, *(sortInfo->myOrder), runlen);
	MergeData *data = new MergeData { &p1, &p2, &tpmms };

	in->ShutDown(); // Stop the Pipe so we don't block
	// Spin up a thread to do the sorting
	int t = pthread_create(&worker, NULL, [] (void* args) -> void* {
		MergeData *data = (MergeData*)args;
		data->tpmms->Merge(data->p1, data->p2);
	}, (void*)data);
	if(t) {
    	cout << "Unable to create thread in SortedDBFile::Flush: " << t << endl;
    }

	// Read in the sorted Records into temp
	Record rec;
	while(sortedrecs.Remove(&rec) != 0) {
		temp.Add(rec);
	}
	temp.Flush(); // Write out any remaining Records

	// Cleanup/Reset state
	Reset();
	Initialize();
}

void SortedDBFile::Reset() {
	cursor -> EmptyItOut();
	cursorIndex = 0;
	if(in != NULL) in->ShutDown();

	// Make sure that BigQ finishes up with in
	Record *rec = new Record();
	if(out != NULL) {
		while(out->Remove(rec) != 0) {/* empty that thing out and wait for BigQ to shut it down */}
	}
	delete in;
	delete out;
	delete rec;
	in = NULL;
	out = NULL;
}

void SortedDBFile::Initialize() {
	bool create = (in == NULL || out == NULL);
	if(in == NULL) in = new Pipe(PIPE_SIZE);
	if(out == NULL) out = new Pipe(PIPE_SIZE);
	if(create) BigQ(*in, *out, *sortInfo->myOrder, *sortInfo->runLength);
}

bool SortedDBFile::BinarySearch(Record &literal, OrderMaker &query) {
	Record rec;
	Page page;
	ComparisonEngine comp;
	return BinarySearch(literal, query, comp, rec, page);
}

bool SortedDBFile::BinarySearch(Record &literal, OrderMaker &query, ComparisonEngine &comp, Record &rec, Page &page) {
	off_t start = cursorIndex;
	off_t end = GetLength() - 1;
	off_t mid;

	int c;
	while(start + 1 < end) {
		mid = (start + end)/2; 
		GetBSTPage(page, mid);
		if(page.GetFirst(&rec) == 0) ++start; 
		else if((c = comp.CompareForSearch(&rec, sortInfo->myOrder, &literal, &query)) < 0) start = mid;
		else end = mid;
	}

	
	if(c >= 0 && (mid > cursorIndex)) --mid;
	return FindValidRecord(literal, query, mid);
}

void SortedDBFile::GetBSTPage(Page &page, off_t index) {
	if(index == cursorIndex) {
		char *bits = new (std::nothrow) char[PAGE_SIZE];
		cursor->ToBinary(bits);
		page.FromBinary(bits);
	}
	else file.GetPage(&page, index);
}

bool SortedDBFile::FindValidRecord(Record &literal, OrderMaker &query, off_t index) {
	Record rec;
	Page page;
	Page buff;
	ComparisonEngine comp;
	return FindValidRecord(literal, query, index, rec, page, buff, comp);
}

bool SortedDBFile::FindValidRecord(Record &literal, OrderMaker &query, off_t index, Record &rec,
		Page &page, Page &buff, ComparisonEngine &comp) {

	int c;

	
	GetBSTPage(page, index);
	while(page.GetFirst(&rec) != 0 && (c = comp.CompareForSearch(&rec, sortInfo->myOrder, &literal, &query)) <= 0) {
		if(c == 0) break;
	}

	if(c < 0 && index < GetLength() - 1) {
		++index;
		GetBSTPage(page, index);
		while(page.GetFirst(&rec) != 0 && (c = comp.CompareForSearch(&rec, sortInfo->myOrder, &literal, &query)) <= 0) {
			if(c == 0) break;
		}
	}

	
	if(c != 0) return false;
	
	buff.Append(&rec); 
	while(page.GetFirst(&rec) != 0) { buff.Append(&rec); }

	char *bits = new (std::nothrow) char[PAGE_SIZE];
	buff.ToBinary(bits);
	cursor->FromBinary(bits);
	cursorIndex = index;

	return true;
}