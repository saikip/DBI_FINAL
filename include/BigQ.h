#ifndef BIGQ_H
#define BIGQ_H
#include <pthread.h>
#include <iostream>
#include <vector>
#include "Pipe.h"
#include "File.h"
#include "Record.h"
#include "PipedPage.h"


using namespace std;

class BigQ {
friend class MockBigQ;
public:
	BigQ();
public:
	BigQ (Pipe &in, Pipe &out, OrderMaker &sortorder, int runlen);
	virtual ~BigQ ();
};

class TPMMS {
friend class TPMMSTest;
friend class MockTPMMS;
friend class MockForPhase1;
public:
	Pipe &in;
	Pipe &out;
	File &file;
	File myFile;
	Page &page;
	Page myPage;
	Record *rec;
	ComparisonEngine &comp;
	ComparisonEngine myComp;
	OrderMaker &order;
	vector<off_t> &runPos;
	vector<off_t> myRunPos;
	vector<Record *> &run;
	vector<Record *> myRun;
	int currRunSizeInBytes;
	int runSizeInBytes;
	const int runlen;
	off_t totalPageCount;

	TPMMS();
	TPMMS(Pipe &_in, Pipe &_out, File &_file, Page &_page, ComparisonEngine &_comp,
			OrderMaker &_order, vector<off_t> &_runPos, vector<Record *> &_run, int &runlen);

	virtual void SortRun();
	virtual void ProcessPages(off_t &totalPageCount);
	virtual bool AddRecord(Record* &rec);
	virtual void Phase1();
	virtual void GetNextRecord(int min, Record **&heads, off_t *&runIndex, Page **&pages, int &runsLeft);
	virtual int FindMin(int size, Record **&heads);
	virtual void Phase2();

public:
	TPMMS(Pipe &in, Pipe &out, OrderMaker &sortorder, int &runlen);
	virtual ~TPMMS();
	virtual void Sort();
	virtual void Merge(PipedPage *p1, PipedPage *p2);
};

void* Work(void *ptr);

#endif
