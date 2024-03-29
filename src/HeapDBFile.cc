#include "../include/HeapDBFile.h"
#include "../include/GenericDBFile.h"
#include "TwoWayList.h"
#include "Record.h"
#include "Schema.h"
#include "File.h"
#include "Comparison.h"
#include "ComparisonEngine.h"
#include "DBFile.h"
#include "Defs.h"
#include <iostream>
#include <string.h>
#include <stdio.h>

HeapDBFile::HeapDBFile():GenericDBFile() {
	cursorIndex = 0;
	lastIndex = 0;
	cursor = NULL;
	last = NULL;
}

HeapDBFile::HeapDBFile(File &file, Input &rfile, Settings &config, ComparisonEngine &comp):
GenericDBFile(file, rfile, config, comp) {
	cursorIndex = 0;
	lastIndex = 0;
	cursor = new Page();
	last = new Page();
}

HeapDBFile::~HeapDBFile () {
	delete cursor;
	delete last;
}

void HeapDBFile::Load (Schema &f_schema, const char *loadpath) {
	Record record;
	Load(f_schema, loadpath, record);
}

void HeapDBFile::Load (Schema &f_schema, const char *loadpath, Record &record) {
	if(!rfile.FileExists(loadpath)) throw std::runtime_error(loadpath + std::string(" could not be found."));
	// File exists
	FILE *file = fopen(loadpath, "r");
	while(record.SuckNextRecord(&f_schema, file)) {
		Add(record);
	}
	fclose(file);
}

void HeapDBFile::MoveFirst () {
	file.AddPage(last, lastIndex); // Write out last page
	if(GetLength() > 0) file.GetPage(cursor, 0); // Get Page if it exists
	else cursor -> EmptyItOut(); // Empty out current Page if no Page exists
	cursorIndex = 0;
}

void HeapDBFile::Add (Record &rec) {
	if(!last->Append(&rec)) {
		file.AddPage(last, lastIndex);
		last->EmptyItOut();
		if(!last->Append(&rec)) throw std::runtime_error("rec exceeds the Page size");
		lastIndex++;
	}
}

int HeapDBFile::GetNext (Record &fetchme) {
	if(GetLength() == 0) {
		return 0;
	}

	if(cursor->GetFirst(&fetchme)) {
		return 1;
	} else {
		cursorIndex++;
		while(cursorIndex < GetLength()) {
			cursor->EmptyItOut();
			file.GetPage(cursor, cursorIndex);
			if(cursor->GetFirst(&fetchme)) {
				return 1;
			}
			cursorIndex++;
		}
		cursorIndex--;
		return 0;
	}
}

int HeapDBFile::GetNext (Record &fetchme, CNF &cnf, Record &literal) {
	while(this->GetNext(fetchme)) {
		if(comp.Compare(&fetchme, &literal, &cnf)) {
			return 1;
		}
	}
	return 0;
}

void HeapDBFile::Flush() {
	file.AddPage(last, lastIndex);
}

void HeapDBFile::Reset() {
	cursorIndex = 0;
	lastIndex = 0;
	cursor->EmptyItOut();
	last->EmptyItOut();
	config.Clear();
}

void HeapDBFile::Initialize() {
	if(GetLength() == 0) { 
		lastIndex = 0;          
	} else {
		file.GetPage(cursor, 0);
		lastIndex = GetLength() - 1;
		file.GetPage(last, lastIndex);
	}
}
