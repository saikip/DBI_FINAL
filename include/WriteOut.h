/*---
 * Title        : Project - 3,
 * Course       : DBI
 * Team Members : Priyam Saikia, Ruchika Mishra
*/
#ifndef INCLUDE_WRITEOUT_H_
#define INCLUDE_WRITEOUT_H_

#include <pthread.h>
#include "RelationalOp.h"
#include "Pipe.h"
#include "Schema.h"
#include "Record.h"
#include "Input.h"

class WriteOut: public RelationalOp {
public:
	WriteOut();
	virtual ~WriteOut();

	void Run (Pipe &inPipe, FILE *outFile, Schema &mySchema);
	virtual void Use_n_Pages (int n);
	virtual void Write(Pipe &inPipe, FILE *outFile, Schema &mySchema);
	virtual void Write(Pipe &inPipe, FILE *outFile, Schema &mySchema, Input &file, Record &rec);
};

struct WriteOutData {
	Pipe &in;
	FILE *out;
	Schema &schema;
	WriteOut &op;
};

#endif /* INCLUDE_WRITEOUT_H_ */
