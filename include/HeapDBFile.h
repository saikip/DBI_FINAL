#ifndef HEAPDBFILE_H
#define HEAPDBFILE_H

#include "TwoWayList.h"
#include "Record.h"
#include "Schema.h"
#include "File.h"
#include "Comparison.h"
#include "ComparisonEngine.h"
#include "Input.h"
#include "Settings.h"
#include "GenericDBFile.h"
#include <sys/stat.h>

class HeapDBFile: public GenericDBFile {
friend class HeapDBFileTest;
friend class MockHeapDBFile;
private:
	off_t cursorIndex;
	off_t lastIndex;
	Page *cursor;		
	Page * last;

	virtual void Load (Schema &myschema, const char *loadpath, Record &record);
	HeapDBFile();

public:
	HeapDBFile(File &file, Input &rfile, Settings &config, ComparisonEngine &comp);
    virtual ~HeapDBFile();

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
