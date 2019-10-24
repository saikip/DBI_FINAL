#include "SortedDBFileTest.h"
#include "../include/File.h"
#include "../include/Input.h"
#include "../include/Comparison.h"
#include "../include/Settings.h"

#include "../include/SortedDBFileTest.h"

/**
 * SortedDBFile::Add should, if in Reading mode, get set to Writing mode and add the record to in.
 * It should also set GetNextState to NoCNF regardless of any other state.
 */
TEST_F(SortedDBFileTest, TestForSortedAdd) {
	StrictMock<MockPipe> in;
	StrictMock<MockPipe> out;
	StrictMock<MockFile> temp;
	StrictMock<MockBigQ> q;
	StrictMock<Record> rec;

	SetIn(&in);
	SetOut(&out);
	SetRWState(Reading);
	//SetGetNextState(NoCNF);

	// Check to make sure the real work gets called.
	EXPECT_CALL(in, Insert(&rec));

	sorteddb->Add(rec);
	EXPECT_EQ(Writing, GetRWState());
	//EXPECT_EQ(NoCNF, GetGetNextState());

	SetIn(NULL);
	SetOut(NULL);
	delete mock;
}


TEST_F(SortedDBFileTest, TestForClose) {
	StrictMock<MockPipe> in;
	StrictMock<MockPipe> out;
	StrictMock<MockFile> temp;
	StrictMock<MockBigQ> q;
	StrictMock<Record> rec;

	SetIn(&in);
	SetOut(&out);
	SetRWState(Writing);
	//SetGetNextState(UseCNF);

	// Check to make sure the real work gets called.
	EXPECT_CALL(in, Insert(&rec));

	sorteddb->Add(rec);
	EXPECT_EQ(Writing, GetRWState());
	//EXPECT_EQ(NoCNF, GetGetNextState());

	SetIn(NULL);
	SetOut(NULL);
	delete mock;
}

/**
 * DBFile:Create should return 0 if startup is NULL.
 */
TEST_F(SortedDBFileTest, TestForSortedCreation) {
	Sequence s1, s2, s3, s4, s5;

	EXPECT_CALL(config, Clear()).
			InSequence(s2, s3, s4, s5);
	EXPECT_CALL(mockFile, Open(0, path)).
			InSequence(s1);
	EXPECT_CALL(rfile, Open(header)).
			InSequence(s2).
			WillOnce(Return(true));
	EXPECT_CALL(config, Clear()).
			InSequence(s2);
	EXPECT_CALL(mockFile, Close()).
			InSequence(s4).
			WillOnce(Return(1));
	EXPECT_CALL(rfile, Close()).
			InSequence(s5).
			WillOnce(Return(true));

	EXPECT_CALL(rfile, FileExists(Pointee(*path))).
			WillRepeatedly(Return(false));
	EXPECT_CALL(rfile, FileExists(Pointee(*header))).
			WillRepeatedly(Return(false));

	EXPECT_CALL(mockFile, GetLength()).
			WillRepeatedly(Return(0));

	EXPECT_EQ(0, file.Create(path, sorted, NULL));
	EXPECT_EQ(NULL, Delegate());
}

/**
 * DBFile:Create should return 0 if startup contains a SortInfo struct with a runLength less than
 * or equal to 0.
 */
TEST_F(SortedDBFileTest, TestForOrderCreation) {
	Sequence s1, s2, s3, s4, s5;
	SortInfo *info = new SortInfo{ new OrderMaker("0 int 5 string 6 int"), new int(-1) };

	EXPECT_CALL(config, Clear()).
			InSequence(s2, s3, s4, s5);
	EXPECT_CALL(mockFile, Open(0, path)).
			InSequence(s1);
	EXPECT_CALL(rfile, Open(header)).
			InSequence(s2).
			WillOnce(Return(true));
	EXPECT_CALL(config, Clear()).
			InSequence(s2);
	EXPECT_CALL(mockFile, Close()).
			InSequence(s4).
			WillOnce(Return(1));
	EXPECT_CALL(rfile, Close()).
			InSequence(s5).
			WillOnce(Return(true));

	EXPECT_CALL(rfile, FileExists(Pointee(*path))).
			WillRepeatedly(Return(false));
	EXPECT_CALL(rfile, FileExists(Pointee(*header))).
			WillRepeatedly(Return(false));

	EXPECT_CALL(mockFile, GetLength()).
			WillRepeatedly(Return(0));

	EXPECT_EQ(0, file.Create(path, sorted, info));
	EXPECT_EQ(NULL, Delegate());
}

TEST_F(SortedDBFileTest, TestForSortedCloseDB) {
	Sequence s1, s2, s3, s4, s5;
	SortInfo *info = new SortInfo{ NULL, new int(5) };

	EXPECT_CALL(config, Clear()).
			InSequence(s2, s3, s4, s5);
	EXPECT_CALL(mockFile, Open(0, path)).
			InSequence(s1);
	EXPECT_CALL(rfile, Open(header)).
			InSequence(s2).
			WillOnce(Return(true));
	EXPECT_CALL(config, Clear()).
			InSequence(s2);
	EXPECT_CALL(mockFile, Close()).
			InSequence(s4).
			WillOnce(Return(1));
	EXPECT_CALL(rfile, Close()).
			InSequence(s5).
			WillOnce(Return(true));

	EXPECT_CALL(rfile, FileExists(Pointee(*path))).
			WillRepeatedly(Return(false));
	EXPECT_CALL(rfile, FileExists(Pointee(*header))).
			WillRepeatedly(Return(false));

	EXPECT_CALL(mockFile, GetLength()).
			WillRepeatedly(Return(0));

	EXPECT_EQ(0, file.Create(path, sorted, info));
	EXPECT_EQ(NULL, Delegate());
}


TEST_F(SortedDBFileTest, TestForSortedGetNext) {
	Sequence s1, s2, s3;
	SortInfo *info = new SortInfo{ new OrderMaker("0 int 5 string 6 int"), new int(10) };
	MakeSortedDBFile(info);

	// s1
	EXPECT_CALL(mockFile, Open(1, path)).
			InSequence(s1);
	EXPECT_CALL(mockFile, GetLength()).
			WillRepeatedly(Return(5));

	// s2
	EXPECT_CALL(rfile, FileExists(Pointee(*path))).
			InSequence(s2).
			WillOnce(Return(true));
	EXPECT_CALL(rfile, FileExists(Pointee(*header))).
			InSequence(s2).
			WillOnce(Return(true));
	EXPECT_CALL(rfile, Open(header)).
			InSequence(s2).
			WillOnce(Return(true));
	EXPECT_CALL(config, Clear()).
			InSequence(s3);
	EXPECT_CALL(config, Read(_)).
			InSequence(s2, s3).
			WillOnce(Return(true));

	EXPECT_CALL(config, GetKey("fType")).
			WillRepeatedly(Return("sorted"));
	EXPECT_CALL(config, GetKey("order")).
			WillRepeatedly(Return("5 int 2 double"));
	EXPECT_CALL(config, GetKey("runLength")).
			WillRepeatedly(Return("10"));

	EXPECT_EQ(1, file.Open(path));
	EXPECT_EQ(typeid(SortedDBFile), typeid(*Delegate()));
	EXPECT_EQ(0, strcmp("5 int 2 double", GetOrder()->ToString().c_str()));
	EXPECT_EQ(10, GetRunLength());
}

TEST_F(SortedDBFileTest, TestForSortedMoveFirst) {
	Sequence s1, s2, s3;
	SortInfo *info = new SortInfo{ new OrderMaker("0 int 5 string 6 int"), new int(5) };

	// s1
	EXPECT_CALL(mockFile, Open(1, path)).
			InSequence(s1);
	EXPECT_CALL(mockFile, GetLength()).
			WillRepeatedly(Return(5));

	// s2
	EXPECT_CALL(rfile, FileExists(Pointee(*path))).
			InSequence(s2).
			WillOnce(Return(true));
	EXPECT_CALL(rfile, FileExists(Pointee(*header))).
			InSequence(s2).
			WillOnce(Return(true));
	EXPECT_CALL(rfile, Open(header)).
			InSequence(s2).
			WillOnce(Return(true));
	EXPECT_CALL(config, Read(_)).
			InSequence(s2, s3).
			WillOnce(Return(true));

	EXPECT_CALL(config, Clear()).
			Times(AtLeast(1));
	EXPECT_CALL(mockFile, Close()). // Arbitrary
			Times(AtMost(1)).
			InSequence(s1, s3).
			WillRepeatedly(Return(5));
	EXPECT_CALL(rfile, Close()).
			InSequence(s2).
			WillOnce(Return(true));
	EXPECT_CALL(config, GetKey("fType")).
			WillRepeatedly(Return("sorted"));
	EXPECT_CALL(config, GetKey("order")).
			WillRepeatedly(Return(""));
	EXPECT_CALL(config, GetKey("runLength")).
			WillRepeatedly(Return("10"));

	EXPECT_EQ(0, file.Open(path));
	EXPECT_EQ(NULL, Delegate());
}

TEST_F(SortedDBFileTest, TestForSortedRunLength) {
	Sequence s1, s2, s3;
	SortInfo *info = new SortInfo{ new OrderMaker("0 int 5 string 6 int"), new int(5) };

	// s1
	EXPECT_CALL(mockFile, Open(1, path)).
			InSequence(s1);
	EXPECT_CALL(mockFile, GetLength()).
			WillRepeatedly(Return(5));

	// s2
	EXPECT_CALL(rfile, FileExists(Pointee(*path))).
			InSequence(s2).
			WillOnce(Return(true));
	EXPECT_CALL(rfile, FileExists(Pointee(*header))).
			InSequence(s2).
			WillOnce(Return(true));
	EXPECT_CALL(rfile, Open(header)).
			InSequence(s2).
			WillOnce(Return(true));
	EXPECT_CALL(config, Read(_)).
			InSequence(s2, s3).
			WillOnce(Return(true));

	EXPECT_CALL(mockFile, Close()). // Arbitrary
			Times(AtMost(1)).
			InSequence(s1, s3).
			WillRepeatedly(Return(5));
	EXPECT_CALL(rfile, Close()).
			InSequence(s2).
			WillOnce(Return(true));

	EXPECT_CALL(config, Clear()).
			Times(AtLeast(1));
	EXPECT_CALL(config, GetKey("fType")).
			WillRepeatedly(Return("sorted"));
	EXPECT_CALL(config, GetKey("order")).
			WillRepeatedly(Return("5 int 2 double"));
	EXPECT_CALL(config, GetKey("runLength")).
			WillRepeatedly(Return(""));

	EXPECT_EQ(0, file.Open(path));
	EXPECT_EQ(NULL, Delegate());
}

TEST_F(SortedDBFileTest, TestForBinarySearchNotNull) {
	StrictMock<MockPage> cursor;
	StrictMock<MockPage> temp;

	SetCursor(&cursor);
	SetCursorIndex(2);

	InSequence seq;
	EXPECT_CALL(cursor, ToBinary(_));
	EXPECT_CALL(temp, FromBinary(NotNull()));

	GetBSTPage(temp, 2);
	SetIn(NULL);
	SetOut(NULL);
	SetCursor(NULL);
	delete mock;
}

TEST_F(SortedDBFileTest, TestForBinarySearchSixRecords) {
	StrictMock<MockPage> cursor;
	StrictMock<MockPage> temp;

	SetCursor(&cursor);
	SetCursorIndex(2);

	EXPECT_CALL(mockFile, GetPage(&temp, 6));

	GetBSTPage(temp, 6);
	SetIn(NULL);
	SetOut(NULL);
	SetCursor(NULL);
	delete mock;
}



