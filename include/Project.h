/*---
 * Title        : Project - 3,
 * Course       : DBI
 * Team Members : Priyam Saikia, Ruchika Mishra
*/
#ifndef INCLUDE_PROJECT_H_
#define INCLUDE_PROJECT_H_

#include <pthread.h>
#include "RelationalOp.h"
#include "Pipe.h"
#include "Record.h"

class Project: public RelationalOp {
public:
	Project();
	virtual ~Project();
	virtual void Run (Pipe &inPipe, Pipe &outPipe, int *keepMe, int numAttsInput, int numAttsOutput);

	virtual void Use_n_Pages (int n);
	virtual void Work (Pipe &inPipe, Pipe &outPipe, int *keepMe, int numAttsInput, int numAttsOutput);

	virtual void Work (Pipe &inPipe, Pipe &outPipe, int *keepMe, int numAttsInput, int numAttsOutput, Record &rec);
};

struct ProjectData {
	Pipe &in;
	Pipe &out;
	int *keepMe;
	int numAttsInput;
	int numAttsOutput;
	Project &op;
};

#endif /* INCLUDE_PROJECT_H_ */
