#ifndef ENTRYPOINT4_1_H
#define ENTRYPOINT4_1_H

//#include "y.tab.h"
#include <iostream>
#include <stdlib.h>
#include "Statistics.h"
#include "ParseTree.h"
#include <math.h>
extern "C" struct YY_BUFFER_STATE *yy_scan_string(const char*);
extern "C" int yyparse(void);
extern struct AndList *final;

using namespace std;
#endif /*ENTRYPOINT4_1_H */




