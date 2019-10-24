/*---
 * Title        : Project - 3,
 * Course       : DBI
 * Team Members : Priyam Saikia, Ruchika Mishra
*/
#ifndef INCLUDE_GROUPBY_H_
#define INCLUDE_GROUPBY_H_

#include "RelationalOp.h"
#include "Record.h"
#include "Function.h"
#include "Pipe.h"
#include "ComparisonEngine.h"


class GroupBy : public RelationalOp {

private:
	int pageLimit;
	void computeAndOutputSum(int intSum, double doubleSum, Function& func,
	Record& mergeWith, Record& mergeInto, OrderMaker &groupAtts, int* attsToKeep, Pipe& outPipe);

public:
	GroupBy();
	virtual ~GroupBy();

    void Run (Pipe &inPipe, Pipe &outPipe, OrderMaker &groupAtts, Function &computeMe);

	virtual void Use_n_Pages (int n);

	virtual void Work (Pipe &inPipe, Pipe &outPipe, OrderMaker &groupAtts, Function &computeMe);

	virtual void Work (Pipe &inPipe, Pipe &outPipe, OrderMaker &groupAtts, Function &computeMe,
		Record &rec, Record &prev, Record& mergeInto, ComparisonEngine &comp);

	virtual int GetPageLimit();
};

struct GroupByData {
	Pipe &inPipe;
	Pipe &outPipe;
	OrderMaker &groupAtts;
	Function &computeMe;
	GroupBy &op;
};

#endif /* INCLUDE_GROUPBY_H_ */