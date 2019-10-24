/*---
 * Title        : Project - 3,
 * Course       : DBI
 * Team Members : Priyam Saikia, Ruchika Mishra
*/
#include "../include/RelationalOp.h"

RelationalOp::RelationalOp() {
	thread_id = 0;
}

RelationalOp::~RelationalOp() {

}

void RelationalOp::WaitUntilDone () { (void) pthread_join(worker, NULL); }
