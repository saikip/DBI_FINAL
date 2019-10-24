/*---
 * Title        : Project - 3,
 * Course       : DBI
 * Team Members : Priyam Saikia, Ruchika Mishra
*/

#ifndef INCLUDE_SUM_H_
#define INCLUDE_SUM_H_

#include "Record.h"
#include "Pipe.h"
#include "RelationalOp.h"
#include "Function.h"

class Sum: public RelationalOp {
public:
	Sum();
	virtual ~Sum();

	virtual void Run (Pipe &inPipe, Pipe &outPipe, Function &computeMe);

	virtual void Use_n_Pages (int n);

	virtual void Work (Pipe &inPipe, Pipe &outPipe, Function &computeMe);

	virtual void Work (Pipe &inPipe, Pipe &outPipe, Function &computeMe, Record &rec);
};

struct SumData {
	Pipe &in;
	Pipe &out;
	Function &computeMe;
	Sum &op;
};

#endif /* INCLUDE_SUM_H_ */
