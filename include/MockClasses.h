#ifndef MOCKCLASSES_H
#define MOCKCLASSES_H

#include <gmock/gmock.h>
#include <vector>
#include <unordered_map>
#include "Record.h"
#include "File.h"
#include "Input.h"
#include "DBFile.h"
#include "Settings.h"
#include "Schema.h"
#include "ComparisonEngine.h"
#include "BigQ.h"
#include "Pipe.h"
#include "SortedDBFile.h"
#include "TreeDBFile.h"
#include "PipedPage.h"
#include "SequentialProcessor.h"
#include "SelectPipe.h"
#include "DuplicateRemoval.h"
#include "Project.h"
#include "SelectFile.h"
#include "WriteOut.h"
#include "Function.h"
#include "Sum.h"
#include "Relation.h"
#include "FindRelation.h"
#include "CompareInput.h"
#include "CreateJoin.h"
#include "GroupBy.h"
#include "Join.h"
#include "Statements.h"
#include "OpNode.h"
#include "SQL.h"
#include "JoinOptimizer.h"

class MockRecord: public Record {
public:
    MOCK_METHOD1(Consume, void(Record *fromMe));
    MOCK_METHOD1(Copy, void(Record *copyMe));
    MOCK_METHOD2(SuckNextRecord, int(Schema *mySchema, FILE *textFile));
    MOCK_METHOD3(Project, void(int *attsToKeep, int numAttsToKeep, int numAttsNow));
    MOCK_METHOD7(MergeRecords, void(Record *left, Record *right, int numAttsLeft,
    int numAttsRight, int *attsToKeep, int numAttsToKeep, int startOfRight));
    MOCK_METHOD1(Print, void(Schema *mySchema));
	MOCK_METHOD1(ToString, string(Schema *mySchema));
    MOCK_METHOD0(Size, int());
};

class MockPage: public Page {
public:
    MOCK_METHOD1(ToBinary, void(char *bits));
    MOCK_METHOD1(FromBinary, void(char *bits));
    MOCK_METHOD1(GetFirst, int(Record *firstOne));
    MOCK_METHOD1(Append, int(Record *addMe));
    MOCK_METHOD0(EmptyItOut, void());
};

class MockFile: public File {
public:
	MOCK_METHOD0(GetLength, off_t());
    MOCK_METHOD2(Open, void(int length, const char *fname));
    MOCK_METHOD2(GetPage, void(Page *putItHere, off_t whichPage));
    MOCK_METHOD2(AddPage, void(Page *addMe, off_t whichPage));
    MOCK_METHOD0(Close, int());
};

class MockInput: public Input {
public:
    MOCK_METHOD1(Open, bool(std::string fname));
	MOCK_METHOD1(Open, bool(FILE *_file));
    MOCK_METHOD0(Close, bool());
    MOCK_METHOD2(Read, int(void* buf, size_t count));
    MOCK_METHOD2(Write, int(const void* buf, size_t count));
    MOCK_METHOD1(ReadLine,  bool(std::string*));
    MOCK_METHOD1(Append, bool(std::string value));
    MOCK_METHOD0(Truncate, bool());
    MOCK_METHOD1(LSeek, void(off_t offset));
    MOCK_METHOD1(FileExists, bool(const char* fileName));
    MOCK_METHOD1(MakeTemp, void(char *scheme));
    MOCK_METHOD2(Rename, int(const char *oldName, const char *newName));
    MOCK_METHOD1(Remove, int(const char *name));
};

class MockSettings: public Settings {
public:
    MOCK_METHOD1(Read, bool(Input &file));
    MOCK_METHOD1(Write, bool(Input &file));
    MOCK_METHOD1(Open, bool(std::string fname));
    MOCK_METHOD0(Close, bool());
    MOCK_METHOD2(AddKey, void(std::string key, std::string value));
    MOCK_METHOD1(GetKey, std::string(std::string key));
    MOCK_METHOD2(ReplaceKey, void(std::string key, std::string value));
	MOCK_METHOD0(Clear, void());
};

class MockDBFile: public DBFile {
public: 
	MOCK_METHOD3(Create, int(const char *fpath, fType file_type, void *startup));
	MOCK_METHOD1(Open, int(const char *fpath));
	MOCK_METHOD0(Close, int());
	MOCK_METHOD2(Load, void(Schema &myschema, const char *loadpath));
	MOCK_METHOD0(MoveFirst, void());
	MOCK_METHOD1(Add, void(Record &addme));
	MOCK_METHOD1(GetNext, int(Record &fetchme));
	MOCK_METHOD3(GetNext, int(Record &fetchme, CNF &cnf, Record &literal));
	MOCK_METHOD0(Reset, void());
};

class MockSchema: public Schema {
	MOCK_METHOD0(GetAtts, Attribute*());
	MOCK_METHOD0(GetNumAtts, int());
	MOCK_METHOD1(Find, int(char *attName));
	MOCK_METHOD1(FindType, Type(char *attName));
	MOCK_METHOD1(GetSortOrder, int(OrderMaker &order));
};

class MockComparisonEngine: public ComparisonEngine {
public:
	MOCK_METHOD3(Compare, int(Record *left, Record *right, OrderMaker *orderUs));
	MOCK_METHOD4(Compare, int(Record *left, OrderMaker *order_left, Record *right, OrderMaker *order_right));
	MOCK_METHOD4(Compare, int(Record *left, Record *right, Record *literal, CNF *myComparison));
	MOCK_METHOD3(Compare, int(Record *left, Record *literal, CNF *myComparison));
	MOCK_METHOD4(CompareForSearch, int(Record *left, OrderMaker *order_left, Record *right, OrderMaker *order_right));

};

class MockBigQ: public BigQ {

};

class MockPipe: public Pipe {
public:
	MOCK_METHOD1(Insert, void(Record *insertMe));
	MOCK_METHOD1(Remove, int(Record *insertMe));
	MOCK_METHOD0(ShutDown, void());
};

class MockTPMMS: public TPMMS {
public:
	MOCK_METHOD0(ShortRun, void());
	MOCK_METHOD0(PageToRun, void());
	MOCK_METHOD1(ProcessPages, void(off_t &totalPageCount));
	MOCK_METHOD1(AddRecord, bool(Record* &rec));
	MOCK_METHOD0(Phase1, void());
	MOCK_METHOD0(Phase2, void());
	MOCK_METHOD0(Sort, void());
	MOCK_METHOD5(GetNextRecord, void(int min, Record **&heads, off_t *&runIndex, Page **&pages, int &runsLeft));
	MOCK_METHOD2(FindMin, int(int size, Record **&heads));
	MOCK_METHOD2(Merge, void(PipedPage *p1, PipedPage *p2));
};

class MockGenericDBFile: public GenericDBFile {
public:
	MOCK_METHOD2(Load, void(Schema &myschema, const char *loadpath));
	MOCK_METHOD0(MoveFirst, void());
	MOCK_METHOD1(Add, void(Record &addme));
	MOCK_METHOD1(GetNext, int(Record &fetchme));
	MOCK_METHOD3(GetNext, int(Record &fetchme, CNF &cnf, Record &literal));
	MOCK_METHOD0(Reset, void());
	MOCK_METHOD0(Flush, void());
	MOCK_METHOD0(Initialize, void());
};

class MockHeapDBFile: public HeapDBFile {
public:
	MOCK_METHOD2(Load, void(Schema &myschema, const char *loadpath));
	MOCK_METHOD0(MoveFirst, void());
	MOCK_METHOD1(Add, void(Record &addme));
	MOCK_METHOD1(GetNext, int(Record &fetchme));
	MOCK_METHOD3(GetNext, int(Record &fetchme, CNF &cnf, Record &literal));
	MOCK_METHOD0(Reset, void());
	MOCK_METHOD0(Flush, void());
	MOCK_METHOD0(Initialize, void());
};
class MockSortedDBFile: public SortedDBFile {
public:
	MOCK_METHOD2(Load, void(Schema &myschema, const char *loadpath));
	MOCK_METHOD0(MoveFirst, void());
	MOCK_METHOD1(Add, void(Record &addme));
	MOCK_METHOD1(GetNext, int(Record &fetchme));
	MOCK_METHOD3(GetNext, int(Record &fetchme, CNF &cnf, Record &literal));
	MOCK_METHOD0(Reset, void());
	MOCK_METHOD0(Flush, void());
	MOCK_METHOD0(Initialize, void());
	MOCK_METHOD1(Flush, void(File &temp));
	MOCK_METHOD1(Flush, void(HeapDBFile &temp));
	MOCK_METHOD2(BinarySearch, bool(Record &literal, OrderMaker &query));
	MOCK_METHOD5(BinarySearch, bool(Record &literal, OrderMaker &query, ComparisonEngine &comp, Record &rec, Page &page));
	MOCK_METHOD2(SortedDBFile, void(Page &page, off_t index));
	MOCK_METHOD3(FindValidRecord, bool(Record &literal, OrderMaker &query, int index));
	MOCK_METHOD7(FindValidRecord, bool(Record &literal, OrderMaker &query, int index, Record &rec, Page &page, Page &buff, ComparisonEngine &comp));
};

class MockSequentialProcessor: public SequentialProcessor {
public:
	MOCK_METHOD1(GetNext, int(Record &rec));
};

class MockSelectPipe: public SelectPipe {
public:
//	virtual void Run (Pipe &inPipe, Pipe &outPipe, CNF &selOp, Record &literal);
	MOCK_METHOD4(Run, void(Pipe &inPipe, Pipe &outPipe, CNF &selOp, Record &literal));
//	virtual void Use_n_Pages (int n);
	MOCK_METHOD1(Use_n_Pages, void(int n));
//	virtual void Select(Pipe &inPipe, Pipe &outPipe, CNF &selOp, Record &literal);
	MOCK_METHOD4(Select, void(Pipe &inPipe, Pipe &outPipe, CNF &selOp, Record &literal));
//	virtual void Select(Pipe &inPipe, Pipe &outPipe, CNF &selOp, Record &literal, ComparisonEngine &comp, Record &rec);
	MOCK_METHOD6(Select, void(Pipe &inPipe, Pipe &outPipe, CNF &selOp, Record &literal, ComparisonEngine &comp, Record &rec));
};

class MockDuplicateRemoval: public DuplicateRemoval {
public:
//	virtual void Run (Pipe &inPipe, Pipe &outPipe, Schema &mySchema);
	MOCK_METHOD3(Run, void(Pipe &inPipe, Pipe &outPipe, Schema &mySchema));
//	virtual void Use_n_Pages (int n);
	MOCK_METHOD1(Use_n_Pages, void(int n));
//	virtual void Remove(Pipe &inPipe, Pipe &outPipe, Schema &mySchema);
	MOCK_METHOD3(Remove, void(Pipe &inPipe, Pipe &outPipe, Schema &mySchema));
//	virtual void Remove(Pipe &inPipe, Pipe &outPipe, Schema &mySchema, Record &rec, Record &prev, ComparisonEngine &comp, OrderMaker &order);
	MOCK_METHOD7(Select, void(Pipe &inPipe, Pipe &outPipe, Schema &mySchema, Record &rec, Record &prev, ComparisonEngine &comp, OrderMaker &order));
//	virtual int GetPageLimit();
	MOCK_METHOD0(GetPageLimit, int());
};

class MockProject: public Project {
public:
//	virtual void Run (Pipe &inPipe, Pipe &outPipe, int *keepMe, int numAttsInput, int numAttsOutput)
	MOCK_METHOD5(Run, void(Pipe &inPipe, Pipe &outPipe, int *keepMe, int numAttsInput, int numAttsOutput));
//	virtual void Use_n_Pages (int n);
	MOCK_METHOD1(Use_n_Pages, void(int n));
//	virtual void Work (Pipe &inPipe, Pipe &outPipe, int *keepMe, int numAttsInput, int numAttsOutput);
	MOCK_METHOD5(Work, void((Pipe &inPipe, Pipe &outPipe, int *keepMe, int numAttsInput, int numAttsOutput)));
//	virtual void Work (Pipe &inPipe, Pipe &outPipe, int *keepMe, int numAttsInput, int numAttsOutput, Record &rec);
	MOCK_METHOD6(Work, void(Pipe &inPipe, Pipe &outPipe, int *keepMe, int numAttsInput, int numAttsOutput, Record &rec));
};

class MockSelectFile: public SelectPipe {
public:
//	virtual void Run (Pipe &inPipe, Pipe &outPipe, CNF &selOp, Record &literal);
	MOCK_METHOD4(Run, void(Pipe &inPipe, Pipe &outPipe, CNF &selOp, Record &literal));
//	virtual void Use_n_Pages (int n);
	MOCK_METHOD1(Use_n_Pages, void(int n));
//	virtual void Select(DBFile &inFile, Pipe &outPipe, CNF &selOp, Record &literal);
	MOCK_METHOD4(Select, void(DBFile &inFile, Pipe &outPipe, CNF &selOp, Record &literal));
//	virtual void Select(DBFile &inFile, Pipe &outPipe, CNF &selOp, Record &literal, ComparisonEngine &comp, Record &rec);
	MOCK_METHOD6(Select, void(DBFile &inFile, Pipe &outPipe, CNF &selOp, Record &literal, ComparisonEngine &comp, Record &rec));
};

class MockWriteOut: public WriteOut {
public:
//	virtual void Run (Pipe &inPipe, FILE *outFile, Schema &mySchema);
	MOCK_METHOD3(Run, void(Pipe &inPipe, FILE *outFile, Schema &mySchema));
//	virtual void Use_n_Pages (int n);
	MOCK_METHOD1(Use_n_Pages, void(int n));
//	virtual void Write(Pipe &inPipe, FILE *outFile, Schema &mySchema);
	MOCK_METHOD3(Write, void(Pipe &inPipe, FILE *outFile, Schema &mySchema));
//	virtual void Write(Pipe &inPipe, FILE *outFile, Schema &mySchema, Input &file, Record &rec);
	MOCK_METHOD5(Write, void(Pipe &inPipe, FILE *outFile, Schema &mySchema, Input &file, Record &rec));
};

class MockFunction: public Function {
public:
//	void GrowFromParseTree (struct FuncOperator *parseTree, Schema &mySchema);
	MOCK_METHOD2(GrowFromParseTree, void(struct FuncOperator *parseTree, Schema &mySchema));
//	Type RecursivelyBuild (struct FuncOperator *parseTree, Schema &mySchema);
	MOCK_METHOD2(RecursivelyBuild, Type(struct FuncOperator *parseTree, Schema &mySchema));
//	void Print ();
	MOCK_METHOD0(Print, void());
//	Type Apply (Record &toMe, int &intResult, double &doubleResult);
	MOCK_METHOD3(Apply, Type(Record &toMe, int &intResult, double &doubleResult));
//	bool ReturnsInt();
	MOCK_METHOD0(ReturnsInt, bool());
};

class MockSum: public Sum {
public:
//	virtual void Run (Pipe &inPipe, Pipe &outPipe, Function &computeMe);
	MOCK_METHOD3(Run, void(Pipe &inPipe, FILE *outFile, Schema &mySchema));
//	virtual void Use_n_Pages (int n);
	MOCK_METHOD1(Use_n_Pages, void(int n));
//	virtual void Work (Pipe &inPipe, Pipe &outPipe, Function &computeMe);
	MOCK_METHOD3(Work, void(Pipe &inPipe, Pipe &outPipe, Function &computeMe));
//	virtual void Work (Pipe &inPipe, Pipe &outPipe, Function &computeMe, Record &rec);
	MOCK_METHOD4(Work, void(Pipe &inPipe, Pipe &outPipe, Function &computeMe, Record &rec));
};

class MockRelation: public Relation {
public:
//	virtual bool Add(Record *rec) = 0;
	MOCK_METHOD1(Add, bool(Record *rec));
//	virtual bool GetNext(Record *&rec) = 0;
	MOCK_METHOD1(GetNext, bool(Record *&rec));
//	virtual void Reset() = 0;
	MOCK_METHOD0(Reset, void());
//	virtual void Clear() = 0;
	MOCK_METHOD0(Clear, void());
//	virtual int MemUsed();
	MOCK_METHOD0(MemUsed, int());
};

class MockFindRelation: public FindRelation {
public:
//	virtual bool Add(Record *rec) = 0;
	MOCK_METHOD1(Add, bool(Record *rec));
//	virtual bool GetNext(Record *&rec) = 0;
	MOCK_METHOD1(GetNext, bool(Record *&rec));
//	virtual void Reset() = 0;
	MOCK_METHOD0(Reset, void());
//	virtual void Clear() = 0;
	MOCK_METHOD0(Clear, void());
//	virtual int MemUsed();
	MOCK_METHOD0(MemUsed, int());
//	virtual void SetMemLimit(int limit);
	MOCK_METHOD1(SetMemLimit, void(int limit));
};

class MockCompareInput: public CompareInput{
public:
//	virtual bool Add(Record *rec) = 0;
	MOCK_METHOD1(Add, bool(Record *rec));
//	virtual bool Add(Pipe &pipe) = 0;
	MOCK_METHOD1(Add, bool(Pipe &pipe));
//	virtual bool Add(Relation &rel) = 0;
	MOCK_METHOD1(Add, bool(Relation &rel));
//	virtual bool GetNext(Record *&rec) = 0;
	MOCK_METHOD1(GetNext, bool(Record *&rec));
//	virtual void Reset() = 0;
	MOCK_METHOD0(Reset, void());
//	virtual void Clear() = 0;
	MOCK_METHOD0(Clear, void());
//	virtual int MemUsed();
	MOCK_METHOD0(MemUsed, int());
};

class MockCreateJoin: public CreateJoin {
public:
//	virtual bool Add(Record *rec) = 0;
	MOCK_METHOD1(Add, bool(Record *rec));
//	virtual bool Populate(Pipe &in);
	MOCK_METHOD1(Populate, bool(Pipe &in));
//	virtual bool GetNext(Record *&rec) = 0;
	MOCK_METHOD1(GetNext, bool(Record *&rec));
//	virtual void Reset() = 0;
	MOCK_METHOD0(Reset, void());
//	virtual void Clear() = 0;
	MOCK_METHOD0(Clear, void());
//	virtual int MemUsed();
	MOCK_METHOD0(MemUsed, int());
//	virtual bool IsMemoryMode();
	MOCK_METHOD0(IsMemoryMode, bool());
};

class MockGroupBy: public GroupBy {
public:
	// void Run (Pipe &inPipe, Pipe &outPipe, OrderMaker &groupAtts, Function &computeMe);
	MOCK_METHOD4(Run, void(Pipe &inPipe, Pipe &outPipe, OrderMaker &groupAtts, Function &computeMe));
	// virtual void Use_n_Pages (int n);
	MOCK_METHOD1(Use_n_Pages, void(int n));
	// virtual void Work (Pipe &inPipe, Pipe &outPipe, OrderMaker &groupAtts, Function &computeMe);
	MOCK_METHOD4(Work, void(Pipe &inPipe, Pipe &outPipe, OrderMaker &groupAtts, Function &computeMe));
	// virtual void Work (Pipe &inPipe, Pipe &outPipe, OrderMaker &groupAtts, Function &computeMe, Record &rec, Record &prev, ComparisonEngine &comp);
	MOCK_METHOD8(Work, void(Pipe &inPipe, Pipe &outPipe, OrderMaker &groupAtts, Function &computeMe,
		Record &rec, Record &prev, Record& mergeInto, ComparisonEngine &comp));
	// virtual int GetPageLimit();1
	MOCK_METHOD0(GetPageLimit, int());
};

class MockJoin: public Join{
public:
//	virtual void Use_n_Pages (int n);
	MOCK_METHOD1(Use_n_Pages, void(int n));
//	virtual void Run (Pipe &inPipeL, Pipe &inPipeR, Pipe &outPipe, CNF &selOp, Record &literal);
	MOCK_METHOD5(Run, void(Pipe &inPipeL, Pipe &inPipeR, Pipe &outPipe, CNF &selOp, Record &literal));
};

class MockStatements : public Statements {
public:
	// virtual bool Combine(Statements &e) = 0;
	MOCK_METHOD1(Combine, bool(Statements& e));

	// virtual double Tuples() = 0;
	MOCK_METHOD0(Tuples, double());

	// virtual double Numerator() = 0;
	MOCK_METHOD0(Numerator, double());

	// virtual double Denominator() = 0;
	MOCK_METHOD0(Denominator, double());
};

class MockSQL: public SQL {
public:
//	virtual const struct FuncOperator* Function() const;
	MOCK_METHOD0(Function, const FuncOperator*());
//	virtual const Statistics& GetStatistics() const;
	MOCK_METHOD0(GetStatistics, const Statistics&());
//	virtual void Parse(const std::string &sql);
	MOCK_METHOD1(Parse, void(const std::string));
//	virtual void Parse();
	MOCK_METHOD0(Parse, SQL_Command());
//	virtual void GetWhere(SelectMap &selects, JoinMap &joins);
	MOCK_METHOD2(GetWhere, void(SelectMap &selects, JoinMap &joins));
//	virtual void GetGroup(std::vector<RelAttPair> &pairs);
	MOCK_METHOD1(GetGroup, void(std::vector<RelAttPair> &pairs));
//	virtual void GetSelect(std::vector<RelAttPair> &pairs);
	MOCK_METHOD1(GetSelect, void(std::vector<RelAttPair> &pairs));
//	virtual void GetTables(std::vector<RelAliasPair> &pairs);
	MOCK_METHOD1(GetTables, void(std::vector<RelAliasPair> &pairs));
//	virtual void GetFunctionAttributes(std::vector<RelAttPair> &pair);
	MOCK_METHOD1(GetFunctionAttributes, void(std::vector<RelAttPair> &pair));
//	virtual bool DistinctAggregate();
	MOCK_METHOD0(DistinctAggregate, bool());
//	virtual bool DistinctSelect();
	MOCK_METHOD0(DistinctSelect, bool());
//	virtual void ParseWhere(struct AndList *where, SelectMap &selects, JoinMap &joins);
	MOCK_METHOD3(ParseWhere, void(struct AndList *where, SelectMap &selects, JoinMap &joins));
//	virtual void ParseNameList(struct NameList *list, std::vector<RelAttPair> &pair);
	MOCK_METHOD2(ParseNameList, void(struct NameList *list, std::vector<RelAttPair> &pair));
//	virtual void ParseFuncOperator(FuncOperator *func, std::vector<RelAttPair> &pair);
	MOCK_METHOD2(ParseFuncOperator, void(FuncOperator *func, std::vector<RelAttPair> &pair));
//	virtual void ParseTableList(TableList *list, std::vector<RelAliasPair> &pairs);
	MOCK_METHOD2(ParseTableList, void(TableList *list, std::vector<RelAliasPair> &pairs));
};

#endif
