/*---
 * Title        : Project - 3,
 * Course       : DBI
 * Team Members : Priyam Saikia, Ruchika Mishra
*/

#ifndef INCLUDE_RELATIONALOP_H_
#define INCLUDE_RELATIONALOP_H_

#include <pthread.h>

class RelationalOp {
protected:
	int thread_id;
	pthread_t worker;

public:
	RelationalOp();
	virtual ~RelationalOp();
	virtual void WaitUntilDone ();
	virtual void Use_n_Pages (int n) = 0;
};

#endif /* INCLUDE_RELATIONALOP_H_ */
