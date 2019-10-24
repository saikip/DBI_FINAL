#include "GenericDBFile.h"
#include "Pipe.h"
#include "BigQ.h"
#include "HeapDBFile.h"
#include "SequentialProcessor.h"

#ifndef INCLUDE_SORTEDDBFILE_H_
#define INCLUDE_SORTEDDBFILE_H_

#define PIPE_SIZE 100

class SortInfo {
public:
	OrderMaker *myOrder;
	int* runLength;
	SortInfo(OrderMaker* _myOrder, int* _runLength): myOrder(_myOrder), runLength(_runLength) {}
	~SortInfo() { delete myOrder; delete runLength; }
};

struct MergeData {
	PipedPage *p1;
	PipedPage *p2;
	TPMMS *tpmms;
};

enum RWState { Reading, Writing };
enum GetNextState { UseCNF, NoCNF };

class SortedDBFile: public GenericDBFile {
friend class SortedDBFileTest;
friend class MockSOrtedDBFile;
friend class PartialSortedDBFileMock;
private:
	SortInfo *sortInfo;
	const char *f_path;
	Pipe *in;
	Pipe *out; 
	RWState rwState; 
	GetNextState getNextState; 
	Page* cursor;
	off_t cursorIndex;
	SequentialProcessor *scanner;

	virtual void Load (Schema &myschema, const char *loadpath, Record &record);

	virtual void Flush(File &temp);

	virtual void Flush(HeapDBFile &temp);

	virtual bool BinarySearch(Record &literal, OrderMaker &query);

	virtual bool BinarySearch(Record &literal, OrderMaker &query, ComparisonEngine &comp, Record &rec, Page &page);

	virtual void GetBSTPage(Page &page, off_t index);

	virtual bool FindValidRecord(Record &literal, OrderMaker &query, off_t index);

	virtual bool FindValidRecord(Record &literal, OrderMaker &query, off_t index, Record &rec, Page &page, Page &buff, ComparisonEngine &comp);

	SortedDBFile();

public:
	SortedDBFile(File &file, Input &rfile, Settings &config, ComparisonEngine &comp, const char *f_path, SortInfo *sortInfo);
    virtual ~SortedDBFile();

	virtual void Load (Schema &myschema, const char *loadpath);

	virtual void MoveFirst ();

	virtual void Add (Record &addme);

	virtual int GetNext (Record &fetchme);

	virtual int GetNext (Record &fetchme, CNF &cnf, Record &literal);

	virtual void Flush();

	virtual void Initialize();

	virtual void Reset();
};

#endif 
