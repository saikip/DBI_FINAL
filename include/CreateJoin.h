/*---
 * Title        : Project - 3,
 * Course       : DBI
 * Team Members : Priyam Saikia, Ruchika Mishra
*/
#ifndef INCLUDE_CREATEJOIN_H_
#define INCLUDE_CREATEJOIN_H_

#include "Relation.h"
#include "Pipe.h"
#include "Defs.h"

class CreateJoin: public Relation {
friend class CreateJoinTest;
protected:
	Relation *delegate;
	bool memory;
public:
	CreateJoin(int memLimit = PAGE_SIZE);
	virtual ~CreateJoin();

	virtual bool Add(Record *rec);
	virtual bool Populate(Pipe &in);
	virtual bool GetNext(Record *&rec);
	virtual void Reset();
	virtual void Clear();
	virtual bool IsMemoryMode();
	virtual int MemUsed();
};

#endif /* INCLUDE_CREATEJOIN_H_ */
