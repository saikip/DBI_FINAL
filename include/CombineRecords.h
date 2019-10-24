#ifndef COMBINERECORDS_H
#define COMBINERECORDS_H

#include "Method.h"
#include "CreateJoin.h"
#include "FindRelation.h"

class CombineRecords : public Method {
friend class CombineRecordsTest;
friend class PartialCombineRecords;
private:

	virtual bool PutInPlace(Pipe &inPipeL, Pipe &inPipeR, Record& tempL, Record& tempR,
		OrderMaker& orderL, OrderMaker& orderR, ComparisonEngine& comp);

	virtual bool FillRightGroup(Pipe& inPipeR, Record& groupRecR, Record& tempR, CreateJoin& relR,
		OrderMaker& orderR, ComparisonEngine& comp);

	virtual bool FillLeftGroup(Pipe& inPipeL, Record& groupRecL, Record& tempL, Record& mergeInto,
		FindRelation& relL, CreateJoin& relR, Pipe& outPipe, int memLimit, OrderMaker& orderL, ComparisonEngine& comp);

	virtual void CombineRelations(FindRelation& relL, CreateJoin& relR, Pipe& outPipe, Record& rec);

	virtual void Exit(Pipe& inPipeL, Pipe& inPipeR, Pipe& outPipe);
public:
	CombineRecords();
	~CombineRecords();

	virtual void Join(Pipe &inPipeL, Pipe &inPipeR, Pipe &outPipe, 
		OrderMaker &orderL, OrderMaker &orderR, int memLimit);
};

#endif /* COMBINERECORDS_H */