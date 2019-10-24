#ifndef DBFILE_H
#define DBFILE_H

#include "HeapDBFile.h"
#include "GenericDBFile.h"
#include "TwoWayList.h"
#include "Record.h"
#include "Schema.h"
#include "File.h"
#include "Comparison.h"
#include "ComparisonEngine.h"
#include "Input.h"
#include "Settings.h"
#include <sys/stat.h>
/*---
 * Title        : Project - 1,
 * Course       : DBI
 * Team Members : Priyam Saikia, Ruchika Mishra
*/
typedef enum {heap, sorted, tree} fType;

// stub DBFile header..replace it with your own DBFile.h 

class DBFile {
	
friend class HeapDBFileTest;
friend class SortedDBFileTest;

private:
	File &file;
	File myFile;
	Input &rfile;
	Input myRFile;
	Settings &config;
	Settings myConfig;
	ComparisonEngine &comp;
	ComparisonEngine myComp;
	GenericDBFile *delegate;

	DBFile(File &file, Input &rfile, Settings &config, ComparisonEngine &comp); // Strictly for Testing.

	virtual void Reset();
	void Initialize();

	virtual void Load (Schema &myschema, const char *loadpath, Record &record);

public:
	DBFile ();
    virtual ~DBFile();

	virtual int Create (const char *fpath, fType file_type, void *startup);
	virtual int Open (const char *fpath);
	virtual int Close ();

	virtual void Load (Schema &myschema, const char *loadpath);

	virtual void MoveFirst ();
	virtual void Add (Record &addme);
	virtual int GetNext (Record &fetchme);
	virtual int GetNext (Record &fetchme, CNF &cnf, Record &literal);
};

#endif
