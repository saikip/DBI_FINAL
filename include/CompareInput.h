/*---
 * Title        : Project - 3,
 * Course       : DBI
 * Team Members : Priyam Saikia, Ruchika Mishra
*/
#ifndef INCLUDE_COMPAREINPUT_H_
#define INCLUDE_COMPAREINPUT_H_

#include "Relation.h"
#include "Record.h"
#include "ComparisonEngine.h"
#include "Settings.h"
#include "Input.h"
#include "File.h"
#include "HeapDBFile.h"
#include "Pipe.h"

class CompareInput: public Relation {
friend class CompareInputTest;

protected:
	Input &rfile;
	Input _Rfile;
	File &file;
	File _File;
	Settings &config;
	Settings _Config;
	ComparisonEngine &comp;
	ComparisonEngine _Comp;
	HeapDBFile &relation;
	HeapDBFile _Relation = HeapDBFile(file, rfile, config, comp);
	char name[9] = { 'F', 'R', 'X', 'X', 'X', 'X', 'X', 'X', '\0' }; //= "CompareInputXXXXXX";

	CompareInput(Input &_rfile, File &_file, Settings &_config, ComparisonEngine &_comp, HeapDBFile &_relation);
	virtual bool Add(Pipe &in, Record &temp);

public:
	CompareInput();
	virtual ~CompareInput();
	virtual bool Add(Record *rec);
	virtual bool Add(Pipe &in);
	virtual bool Add(Relation &rel);
	virtual bool GetNext(Record *&rec);
	virtual void Reset();
	virtual void Clear();
	virtual int MemUsed();
};


#endif /* INCLUDE_COMPAREINPUT_H_ */
