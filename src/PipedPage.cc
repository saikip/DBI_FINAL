/*---
 * Title        : Project - 2b,
 * Course       : DBI
 * Team Members : Priyam Saikia, Ruchika Mishra
*/

#include <unistd.h>
#include "File.h"
#include "../include/PipedPage.h"
#include "../include/HeapDBFile.h"
#include "../include/Input.h"
#include "../include/Settings.h"


#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <iostream>
#include <stdlib.h>

PipedPage :: PipedPage (): recs(recs) {}

PipedPage :: PipedPage (Pipe &_recs, File *_file): recs(_recs) {
	pthread_t worker;
	PipedPageData *data = new PipedPageData {_recs, _file};

	int ret = pthread_create(&worker, NULL, [] (void* args) -> void* {
		PipedPageData *data = (PipedPageData*) args;
		Input rfile;
		Settings config;
		ComparisonEngine comp;
		HeapDBFile heap = HeapDBFile(*(data->file), rfile, config, comp);
		Record rec = Record();

		while(heap.GetNext(rec) != 0) {
			data->recs.Insert(&rec);
		}
		
		data->recs.ShutDown();
		data->file->Close();
		data -> file = NULL;
		delete data;
	}, (void*)data);
	if(ret) {
    	cout << "ERROR" << ret << endl;
    }
}

PipedPage :: PipedPage (Pipe &_recs): recs(_recs) {}
PipedPage :: ~PipedPage () {}

void PipedPage :: EmptyItOut () {
	recs.ShutDown();
}

int PipedPage :: GetFirst (Record *firstOne) {
	return recs.Remove(firstOne);
}

int PipedPage :: Append (Record *addMe) {
	recs.Insert(addMe);
	return 1;
}

void PipedPage :: ToBinary (char *bits) {}
void PipedPage :: FromBinary (char *bits) {}
