/*---
 * Title        : Project - 3,
 * Course       : DBI
 * Team Members : Priyam Saikia, Ruchika Mishra
*/
#include "CreateJoin.h"
#include "FindRelation.h"
#include "CompareInput.h"

CreateJoin::CreateJoin(int memLimit): Relation(memLimit) {
	memory = true;
	delegate = new FindRelation(memLimit);
}

CreateJoin::~CreateJoin() {
	delete delegate;
}

bool CreateJoin::Add(Record *rec) {
	if(!delegate->Add(rec)) {
		memory = false;
		CompareInput*temp = new CompareInput();
		temp->Add(*delegate); 
		delete delegate;
		delegate = temp;
		delegate->Add(rec);
	}
	return memory;
}

bool CreateJoin::Populate(Pipe &in) {
	Record rec;
	while(in.Remove(&rec)) {
		Add(&rec);
	}
	return memory;
}

bool CreateJoin::GetNext(Record *&rec) {
	return delegate->GetNext(rec);
}

void CreateJoin::Reset() {
	return delegate->Reset();
}

void CreateJoin::Clear() {
	if(!memory) {
		delete delegate;
		delegate = new FindRelation(memLimit);
		memory = true;
	}
	else delegate->Clear();
}

bool CreateJoin::IsMemoryMode() {
	return memory;
}

int CreateJoin::MemUsed() {
	return delegate->MemUsed();
}
