#ifndef METHOD_H
#define METHOD_H

#include "Pipe.h"
#include "Comparison.h"
#include "Record.h"

class Method {
public:
	virtual void Join(Pipe &inPipeL, Pipe &inPipeR, Pipe &outPipe,
		OrderMaker &orderL, OrderMaker &orderR, int memLimit) = 0;
};

#endif /* METHOD_H */