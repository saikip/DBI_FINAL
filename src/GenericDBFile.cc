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

GenericDBFile::GenericDBFile(): file(myFile), rfile(myRFile), config(myConfig), comp(myComp){}

GenericDBFile::GenericDBFile(File &_file, Input &_rfile, Settings &_config,
		ComparisonEngine &_comp): file(_file), rfile(_rfile), config(_config), comp(_comp) {

}

GenericDBFile::~GenericDBFile() {

}

int GenericDBFile::GetLength() {
	off_t zero = 0;
	return max(zero, file.GetLength()-1);
}

void  GenericDBFile::Load (Schema &myschema, const char *loadpath, Record &record){}

void GenericDBFile::MoveFirst (){}

void GenericDBFile::Add (Record &addme){}

int GenericDBFile::GetNext (Record &fetchme){return 0;}

int GenericDBFile::GetNext (Record &fetchme, CNF &cnf, Record &literal){return 0;}

void GenericDBFile::Flush(){}

void GenericDBFile::Initialize(){}

void GenericDBFile::Reset(){}
