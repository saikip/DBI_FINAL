/*---
 * Title        : Project - 3,
 * Course       : DBI
 * Team Members : Priyam Saikia, Ruchika Mishra
*/

#ifndef INCLUDE_SELECTFILE_H_
#define INCLUDE_SELECTFILE_H_

#include <pthread.h>
#include "RelationalOp.h"
#include "Pipe.h"
#include "Comparison.h"
#include "ComparisonEngine.h"
#include "DBFile.h"

class SelectFile: public RelationalOp {
public:
	SelectFile();
	virtual ~SelectFile();

	virtual void Run (DBFile &inFile, Pipe &outPipe, CNF &selOp, Record &literal);
	virtual void Use_n_Pages (int n);
	virtual void Select(DBFile &inFile, Pipe &outPipe, CNF &selOp, Record &literal);
	virtual void Select(DBFile &inFile, Pipe &outPipe, CNF &selOp, Record &literal, ComparisonEngine &comp, Record &rec);
};

struct SelectFileData {
	DBFile &in;
	Pipe &out;
	CNF &selOp;
	Record &literal;
	SelectFile &op;
};

#endif /* INCLUDE_SELECTFILE_H_ */
