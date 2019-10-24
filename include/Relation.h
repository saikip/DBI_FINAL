/*---
 * Title        : Project - 3,
 * Course       : DBI
 * Team Members : Priyam Saikia, Ruchika Mishra
*/
#ifndef INCLUDE_RELATION_H_
#define INCLUDE_RELATION_H_

#include "Defs.h"
#include "Record.h"

class Relation {
protected:
	int memLimit;

public:
	Relation(int _memLimit = PAGE_SIZE) { memLimit = _memLimit; }
	virtual ~Relation() {};
	virtual bool Add(Record *rec) { return true; }
	virtual bool GetNext(Record *&rec) { return true; }
	virtual void Reset() {}
	virtual void Clear() {}
	virtual int MemUsed() {return 0;}
};


#endif /* INCLUDE_RELATION_H_ */
