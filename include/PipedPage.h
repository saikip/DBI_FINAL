#ifndef INCLUDE_PIPEDPAGE_H_
#define INCLUDE_PIPEDPAGE_H_

#include "File.h"
#include "Pipe.h"

struct PipedPageData {
	Pipe &recs;
	File *file;
};

class PipedPage: public Page {
private:
	Pipe &recs;
public:
	PipedPage ();
	PipedPage (Pipe &recs);

	PipedPage (Pipe &_recs, File *file);
	virtual ~PipedPage ();

	virtual void ToBinary (char *bits);

	virtual void FromBinary (char *bits);

	virtual int GetFirst (Record *firstOne);

	virtual int Append (Record *addMe);

	virtual void EmptyItOut ();
};




#endif 
