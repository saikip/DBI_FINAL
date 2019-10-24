/*---
 * Title        : Project - 3,
 * Course       : DBI
 * Team Members : Priyam Saikia, Ruchika Mishra
*/
#ifndef INCLUDE_JOIN_H_
#define INCLUDE_JOIN_H_

#include <pthread.h>
#include "RelationalOp.h"
#include "Pipe.h"
#include "Comparison.h"
#include "ComparisonEngine.h"
#include "Record.h"
#include "CreateJoin.h"
#include "FindRelation.h"
#include "Relation.h"
#include "CombineRecords.h"

class Join: public RelationalOp {
friend class JoinTest;
friend class PartialMockJoin;
protected:
	int pageLimit;
	int memLimit;
	void Work(Pipe &inPipeL, Pipe &inPipeR, Pipe &outPipe, CNF &selOp, Record &literal, ComparisonEngine &comp, OrderMaker &orderL, OrderMaker &orderR);

	void SortMergeJoin(Pipe &pipeL, Pipe &pipeR, Pipe &outPipe, OrderMaker &orderL, OrderMaker &orderR, CreateJoin &relR, CreateJoin &relS, Record &tempL, Record &tempR);

	void LargeJoin(Pipe &pipeL, Pipe &pipeR, Pipe &outPipe, CNF &selOp, Record &literal, CreateJoin &relS, FindRelation &relR, Record &rec);

	virtual void CombineRelations(Relation &R, Relation &S, Record &rec, Pipe &out);

	virtual void CombineRelations(Relation &R, Relation &S, Record &rec, Pipe &out, ComparisonEngine &comp, Record &literal, CNF &cnf);

public:
	Join();
	virtual ~Join();

	virtual void Use_n_Pages (int n);

	virtual void Run (Pipe &inPipeL, Pipe &inPipeR, Pipe &outPipe, CNF &selOp, Record &literal);

	virtual void Work(Pipe &inPipeL, Pipe &inPipeR, Pipe &outPipe, CNF &selOp, Record &literal);

	virtual void SortMergeJoin(Pipe &pipeL, Pipe &pipeR, Pipe &outPipe, OrderMaker &orderL, OrderMaker &orderR);

	virtual void LargeJoin(Pipe &pipeL, Pipe &pipeR, Pipe &outPipe, CNF &selOp, Record &literal);
};

struct JoinData {
	Pipe &inL;
	Pipe &inR;
	Pipe &out;
	CNF &selOp;
	Record &literal;
	Join &op;
};

#endif /* INCLUDE_JOIN_H_ */
