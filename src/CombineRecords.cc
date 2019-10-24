#include "CombineRecords.h"

CombineRecords::CombineRecords() {}

CombineRecords::~CombineRecords() {}

bool CombineRecords::PutInPlace(Pipe &inPipeL, Pipe &inPipeR, Record& tempL, Record& tempR,
		OrderMaker& orderL, OrderMaker& orderR, ComparisonEngine& comp) { 
	while(true) {
		int compVal = comp.Compare(&tempL, &orderL, &tempR, &orderR);
		if(compVal == 0) {
			return false;
		} else if(compVal < 0) { 
			if(inPipeL.Remove(&tempL) == 0) { 
				return true;
			}
		} else { 
			if(inPipeR.Remove(&tempR) == 0) { 
				return true;
			}
		}
	}
}

bool CombineRecords::FillRightGroup(Pipe& inPipeR, Record& groupRec, Record& tempR, CreateJoin& relR,
		OrderMaker& orderR, ComparisonEngine& comp) {
	while(true) {
		if(inPipeR.Remove(&tempR) == 0) { 
			return true;
		} else { 
			if(comp.Compare(&groupRec, &tempR, &orderR) == 0) { 
				relR.Add(&tempR);
			} else { 
				return false;
			}
		}
	}
}

bool CombineRecords::FillLeftGroup(Pipe& inPipeL, Record& groupRecL, Record& tempL, Record& mergeInto,
		FindRelation& relL, CreateJoin& relR, Pipe& outPipe, int memLimit, OrderMaker& orderL, ComparisonEngine& comp) {
	relL.SetMemLimit(memLimit - relR.MemUsed());

	while(true) {
		if(inPipeL.Remove(&tempL) == 0) { 
			CombineRelations(relL, relR, outPipe, mergeInto);
			return true;
		} else if(comp.Compare(&groupRecL, &tempL, &orderL) == 0) { 
			if(relL.Add(&tempL)) {
			} else {
				CombineRelations(relL, relR, outPipe, mergeInto);
				relL.Clear();
				relL.Add(&tempL);
			}
		} else { 
			CombineRelations(relL, relR, outPipe, mergeInto);
			return false;
		}
	}
}

void CombineRecords::CombineRelations(FindRelation& relL, CreateJoin& relR, Pipe& outPipe, Record& mergeInto) {
	relL.Reset();
	relR.Reset();
	Record tempL, tempR;
	Record* recL = &tempL;
	Record* recR = &tempR;
	relL.GetNext(recL);
	relR.GetNext(recR);
	
	int numAttsLeft = recL->NumAtts();
	int numAttsRight = recR->NumAtts();
	int numAttsToKeep = numAttsLeft + numAttsRight;
	int startOfRight = numAttsLeft;
	int attsToKeep[numAttsLeft + numAttsRight];

	for(int i = 0; i < numAttsLeft; i++) { attsToKeep[i] = i; }
	for(int i = 0; i < numAttsRight; i++) { attsToKeep[i + numAttsLeft] = i; }

	do {
		do {
			mergeInto.MergeRecords(recL, recR, numAttsLeft, numAttsRight,
				attsToKeep, numAttsToKeep, startOfRight);
			outPipe.Insert(&mergeInto);		
		} while(relL.GetNext(recL));
		relL.Reset(); 
		relL.GetNext(recL); 
	} while(relR.GetNext(recR));
}

void CombineRecords::Exit(Pipe& inPipeL, Pipe& inPipeR, Pipe& outPipe) {
	Record rec;
	while(inPipeL.Remove(&rec)) {
		// do nothing
	}
	while(inPipeR.Remove(&rec)) {
		// do nothing
	}
	outPipe.ShutDown();
}

void CombineRecords::Join(Pipe &inPipeL, Pipe &inPipeR, Pipe &outPipe, 
	OrderMaker &orderL, OrderMaker &orderR, int memLimit) {
	
	Record nextL;
	Record nextR;
	if(inPipeL.Remove(&nextL) == 0) {
		return Exit(inPipeL, inPipeR, outPipe);
	}

	if(inPipeR.Remove(&nextR) == 0) {
		return Exit(inPipeL, inPipeR, outPipe);
	}

	FindRelation relL;
	CreateJoin relR = CreateJoin(memLimit);
	ComparisonEngine comp;

	bool leftDone = false;
	bool rightDone = false;
	while(true) {
		if(PutInPlace(inPipeL, inPipeR, nextL, nextR, orderL, orderR, comp)) {
			return Exit(inPipeL, inPipeR, outPipe);
		}

		Record copyR;
		Record tempR;
		copyR.Copy(&nextR);
		relR.Add(&copyR);
		if(FillRightGroup(inPipeR, nextR, tempR, relR, orderR, comp)) {
			rightDone = true;
		}

		Record copyL;
		Record tempL;
		Record mergeInto;
		copyL.Copy(&nextL);
		relL.Add(&copyL);
		if(FillLeftGroup(inPipeL, nextL, tempL, mergeInto,
			relL, relR, outPipe, memLimit, orderL, comp)) {
			leftDone = true;
		}

		if(leftDone || rightDone) {
			return Exit(inPipeL, inPipeR, outPipe);
		}

		relL.Clear();
		relR.Clear();
		nextL.Consume(&tempL);
		nextR.Consume(&tempR);
	}
}