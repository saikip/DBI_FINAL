/*---
 * Title        : Project - 3,
 * Course       : DBI
 * Team Members : Priyam Saikia, Ruchika Mishra
*/
#ifndef INCLUDE_FINDRELATION_H_
#define INCLUDE_FINDRELATION_H_

#include "Relation.h"
#include <vector>
#include "Defs.h"

class FindRelation: public Relation {
friend class FindRelationTest;

private:
	std::vector<Record*> relation;
	int size;
	int memUsed;
	int index;

public:
	FindRelation(int memLimit = PAGE_SIZE);
	virtual ~FindRelation();

	virtual bool Add(Record *rec);

	virtual bool GetNext(Record *&rec);

	virtual void Reset();

	virtual void Clear();

	virtual int MemUsed();

	virtual void SetMemLimit(int limit);
};

#endif /* INCLUDE_FINDRELATION_H_ */
