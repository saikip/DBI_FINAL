#ifndef MISKIADBFILE_H
#define MISKIADBFILE_H

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
class GenericDBFile {
friend class DBFile;
friend class MockGenericDBFile;
protected:
	File &file;
	File myFile;
	Input &rfile;
	Input myRFile;
	Settings &config;
	Settings myConfig;
	ComparisonEngine &comp;
	ComparisonEngine myComp;

	int GetLength();
	virtual void Load (Schema &myschema, const char *loadpath, Record &record);

public:
	GenericDBFile();

	GenericDBFile(File &_file, Input &_rfile, Settings &_config, ComparisonEngine &_comp);

    virtual ~GenericDBFile();
	virtual void MoveFirst ();
	virtual void Add (Record &addme);
	virtual int GetNext (Record &fetchme);
	virtual int GetNext (Record &fetchme, CNF &cnf, Record &literal);
	virtual void Flush();
	virtual void Initialize();
	virtual void Reset();
};

#endif
