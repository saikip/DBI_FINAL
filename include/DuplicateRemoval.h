/*---
 * Title        : Project - 3,
 * Course       : DBI
 * Team Members : Priyam Saikia, Ruchika Mishra
*/
#ifndef INCLUDE_DUPLICATEREMOVAL_H_
#define INCLUDE_DUPLICATEREMOVAL_H_

#include <pthread.h>
#include "RelationalOp.h"
#include "Pipe.h"
#include "Comparison.h"
#include "ComparisonEngine.h"

class DuplicateRemoval: public RelationalOp {
private:
	int pageLimit;
public:
	DuplicateRemoval();
	virtual ~DuplicateRemoval();
	virtual void Run (Pipe &inPipe, Pipe &outPipe, Schema &mySchema);
	virtual void Run (Pipe &inPipe, Pipe &outPipe, Schema &mySchema, Schema &duplicates);
	virtual void Use_n_Pages (int n);
	virtual void Remove(Pipe &inPipe, Pipe &outPipe, Schema &mySchema);
	virtual void Remove(Pipe &inPipe, Pipe &outPipe, Schema &mySchema, Schema &duplicates);
	virtual void Remove(Pipe &inPipe, Pipe &outPipe, Schema &mySchema, Record &rec, Record &prev, ComparisonEngine &comp, OrderMaker &order);
	virtual int GetPageLimit();
};

struct DuplicateRemovalData {
	Pipe &in;
	Pipe &out;
	Schema &schema;
	Schema &duplicates;
	DuplicateRemoval &op;
};


#endif /* INCLUDE_DUPLICATEREMOVAL_H_ */
