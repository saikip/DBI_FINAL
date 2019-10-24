/*---
 * Title        : Project - 3,
 * Course       : DBI
 * Team Members : Priyam Saikia, Ruchika Mishra
*/
#include "../include/CompareInput.h"
#include "Defs.h"

CompareInput::CompareInput(): Relation(), rfile(_Rfile), file(_File), config(_Config),
	comp(_Comp), relation(_Relation) {
	rfile.MakeTemp(name);
	file.Open(0, name);
}

CompareInput::CompareInput(Input &_rfile, File &_file, Settings &_config, ComparisonEngine &_comp,
		HeapDBFile &_relation): Relation(), rfile(_rfile), file(_file), config(_config),
				comp(_comp), relation(_relation) {
	rfile.MakeTemp(name);
	file.Open(0, name);
}

CompareInput::~CompareInput() {
	file.Close();
	if(rfile.Remove(name) != 0) throw runtime_error("CompareInput:: failed to remove temporary file");
}

bool CompareInput::Add(Record *rec) {
	relation.Add(*rec);
	return true;
}

bool CompareInput::Add(Pipe &in) {
	Record rec;
	return Add(in, rec);
}

bool CompareInput::Add(Pipe &in, Record &temp) {
	while(in.Remove(&temp)) {
		relation.Add(temp);
	}
	return true;
}

bool CompareInput::Add(Relation &rel) {
	Record temp;
	Record *rec = &temp;
	rel.Reset(); // Make sure to get all the Records
	while(rel.GetNext(rec)) {
		relation.Add(*rec);
	}
	return true;
}

bool CompareInput::GetNext(Record *&rec) {
	return relation.GetNext(*rec);
}

void CompareInput::Reset() {
	relation.MoveFirst();
}

void CompareInput::Clear() {
	file.Close();
	file.Open(0, name);
	relation.MoveFirst();
}

int CompareInput::MemUsed() {
	return PAGE_SIZE;
}
