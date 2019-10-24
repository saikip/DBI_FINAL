/*---
 * Title        : Project - 3,
 * Course       : DBI
 * Team Members : Priyam Saikia, Ruchika Mishra
*/

#ifndef INCLUDE_SELECTPIPE_H_
#define INCLUDE_SELECTPIPE_H_

#include <pthread.h>
#include "RelationalOp.h"
#include "Pipe.h"
#include "Comparison.h"
#include "ComparisonEngine.h"

class SelectPipe : public RelationalOp {
public:
	SelectPipe();
	virtual ~SelectPipe();

	virtual void Run (Pipe &inPipe, Pipe &outPipe, CNF &selOp, Record &literal);
	virtual void Use_n_Pages (int n);
	virtual void Select(Pipe &inPipe, Pipe &outPipe, CNF &selOp, Record &literal);
	virtual void Select(Pipe &inPipe, Pipe &outPipe, CNF &selOp, Record &literal, ComparisonEngine &comp, Record &rec);
};

struct SelectPipeData {
	Pipe &in;
	Pipe &out;
	CNF &selOp;
	Record &literal;
	SelectPipe &op;
};

#endif /* INCLUDE_SELECTPIPE_H_ */
