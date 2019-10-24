/*---
 * Title        : Project - 3,
 * Course       : DBI
 * Team Members : Priyam Saikia, Ruchika Mishra
*/
#include "../include/FindRelation.h"

FindRelation::FindRelation(int memLimit): Relation(memLimit) {
	size = 0;
	memUsed = 0;
	index = 0;
}

FindRelation::~FindRelation() {
	Clear();
	relation.clear();
}

bool FindRelation::Add(Record *rec) {
	if(rec->Size() + memUsed <= memLimit || size == 0) {
		memUsed += rec->Size();
		++size;

		// Check if vector needs to be resized
		if(relation.size() < size) relation.push_back(NULL);

		Record *temp = new Record;
		temp->Consume(rec);
		relation[size-1] = temp;
		return true;
	}
	return false;
}

bool FindRelation::GetNext(Record *&rec) {
	if(index < size) {
		rec = relation[index];
		++index;
		return true;
	}
	return false;
}

void FindRelation::Reset() {
	index = 0;
}

void FindRelation::Clear() {
	for(int i = 0; i < relation.size(); i++) {
		delete relation[i];
		relation[i] = NULL;
	}
	index = 0;
	size = 0;
	memUsed = 0;
}

int FindRelation::MemUsed() {
	return memUsed;
}

void FindRelation::SetMemLimit(int limit) {
	memLimit = limit;
}
