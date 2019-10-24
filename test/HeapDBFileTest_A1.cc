#include "../include/HeapDBFileTest.h"
#include "../include/Schema.h"
#include "../include/Input.h"
#include <sstream>
#include <unistd.h>
#include <stdio.h>
#include <string>

TEST_F(HeapDBFileTest, TestForLoad) {
	StrictMock<MockRecord> record;
	SetDBToheapdb();
	SetLast(last);
	string fpath = "jkdlkjfslkdjfsdf";
	char *path = (char *)fpath.c_str();
	Input temp;
	temp.Open(fpath);
	temp.Append("junk");
	temp.Close();
	{
		InSequence seq;
		EXPECT_CALL(record, SuckNextRecord(&schema, NotNull())).
				Times(4).
				WillRepeatedly(Return(1));
		EXPECT_CALL(record, SuckNextRecord(&schema, NotNull())).
				WillOnce(Return(0));
	}
	EXPECT_CALL(last, Append(&record)).
			Times(4).
			WillRepeatedly(Return(1));
	EXPECT_CALL(rfile, FileExists(Pointee(*path))).
			Times(AtLeast(1)).
			WillRepeatedly(Return(true));
	this->Load(schema, path, record);
	remove(path);
	SetLastNull();
}

TEST_F(HeapDBFileTest, HeapDBCreationTest) {
	Sequence s1, s2, s3;
	SetLast(last);
	SetCursor(cursor);

	EXPECT_CALL(config, Clear()).
			InSequence(s2, s3);
	EXPECT_CALL(mockFile, Open(0, path)).
			InSequence(s1);
	EXPECT_CALL(rfile, Open(header)).
			InSequence(s2).
			WillOnce(Return(true));
	EXPECT_CALL(config, AddKey("fType", "heap")).
			InSequence(s3);
	EXPECT_CALL(config, Write(Ref(rfile))).
			InSequence(s2, s3).
			WillOnce(Return(true));

	EXPECT_CALL(rfile, FileExists(Pointee(*path))).
			WillRepeatedly(Return(false));
	EXPECT_CALL(rfile, FileExists(Pointee(*header))).
			WillRepeatedly(Return(false));

	EXPECT_CALL(mockFile, GetLength()).
			WillRepeatedly(Return(0));

	EXPECT_EQ(1, file.Create(path, heap, NULL));
	EXPECT_EQ(0, CursorIndex());
	EXPECT_EQ(0, LastIndex());
	EXPECT_EQ(typeid(HeapDBFile), typeid(*Delegate()));
	SetLastNull();
	SetCursorNull();
}

TEST_F(HeapDBFileTest, OpenIfExist) {
	SetCursor(cursor);
	SetLast(last);

	Sequence s1, s2, s3;

	EXPECT_CALL(mockFile, Open(1, path)).
			InSequence(s1);
	EXPECT_CALL(mockFile, GetLength()).
			WillRepeatedly(Return(5));

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
			InSequence(s2).
			WillOnce(Return("heap"));
	EXPECT_CALL(mockFile, GetPage(_, 3)).
			Times(1);
	EXPECT_CALL(mockFile, GetPage(_, 0)).
			Times(1);
	
	EXPECT_EQ(1, file.Open(path));
	EXPECT_EQ(0, GetDelegateCursorIndex());
	EXPECT_EQ(3, GetDelegateLastIndex());
	EXPECT_EQ(typeid(HeapDBFile), typeid(*Delegate()));

	remove(path);
	remove(header);
	SetCursorNull();
	SetLastNull();
}

TEST_F(HeapDBFileTest, TestForMoveFirst) {
	SetCursorIndex(4);
	SetLastIndex(4);
	SetCursor(cursor);
	SetLast(last);
	SetDBToheapdb();

	EXPECT_CALL(mockFile, GetPage(&cursor, 0));
	EXPECT_CALL(mockFile, AddPage(&last, 4));

	EXPECT_CALL(mockFile, GetLength()).
				WillRepeatedly(Return(4));

	file.MoveFirst();
	EXPECT_EQ(0, CursorIndex());
	EXPECT_EQ(4, LastIndex());

	SetCursorNull();
	SetLastNull();
	SetDBNull();
}

TEST_F(HeapDBFileTest, TestForGetNext) {
	
	SetCursor(cursor);
	SetLast(last);
	SetDBToheapdb();

	Record* r;

	EXPECT_CALL(mockFile, GetLength()).
			WillRepeatedly(Return(6));
	EXPECT_CALL(cursor, GetFirst(r)).
			WillOnce(Return(1));

	EXPECT_EQ(1, file.GetNext(*r));
	EXPECT_EQ(0, CursorIndex());

	SetCursorNull();
	SetLastNull();
}

TEST_F(HeapDBFileTest, TestForCNF) {

	SetDBToheapdb();
	SetCursor(cursor);
	SetLast(last);

	Record* r;
	Record* literal;
	CNF* cnf;

	EXPECT_CALL(mockFile, GetLength()).
			WillRepeatedly(Return(4));
	EXPECT_CALL(cursor, GetFirst(r)).
			WillRepeatedly(Return(1));
	EXPECT_CALL(comp, Compare(r, literal, cnf)).
			WillRepeatedly(Return(1));
	
	EXPECT_EQ(1, file.GetNext(*r, *cnf, *literal));

	SetCursorNull();
	SetLastNull();
}


TEST_F(HeapDBFileTest, CloseDBFile) {
	SetDBToheapdb();
	SetCursorIndex(5);
	SetLastIndex(6);

	MockPage *cursor = new StrictMock<MockPage>;
	MockPage *last = new StrictMock<MockPage>;
	SetCursorNull();
	SetLastNull();
	SetCursorP(cursor);
	SetLastP(last);
	MockPage &lastRef = *last;

	Sequence s1, s2;
	EXPECT_CALL(config, Write(Ref(rfile))).
			InSequence(s1, s2).
			WillOnce(Return(true));
	EXPECT_CALL(config, Clear()).
			Times(AtLeast(1)).
			InSequence(s1);
	EXPECT_CALL(rfile, Close()).
			InSequence(s2).
			WillOnce(Return(true));

	Sequence s3;
	EXPECT_CALL(mockFile, AddPage(&lastRef, 6)).
			InSequence(s3);
	EXPECT_CALL(*last, EmptyItOut()).
			InSequence(s3);

	// Order doesn't matter
	EXPECT_CALL(*cursor, EmptyItOut());
	EXPECT_CALL(mockFile, Close());


	EXPECT_EQ(1, file.Close());
	EXPECT_EQ(NULL, Delegate());

	SetDBNull();
}

TEST_F(HeapDBFileTest, AddToEnd) {
	SetDBToheapdb();
	SetCursorIndex(0);
	SetLastIndex(5);
	SetCursor(cursor);
	SetLast(last);
	StrictMock<MockRecord> record;
	Sequence s1;
	EXPECT_CALL(last, Append(&record)).
			InSequence(s1).
			WillOnce(Return(1));

	file.Add(record);
	EXPECT_EQ(0, CursorIndex());
	EXPECT_EQ(5, LastIndex());
	SetCursorNull();
	SetLastNull();
	SetDBNull();
}

TEST_F(HeapDBFileTest, PageTest) {
	SetDBToheapdb();
	SetCursorIndex(5);
	SetLastIndex(5);
	SetCursor(cursor);
	SetLast(last);
	StrictMock<MockRecord> record;

	InSequence seq;
	EXPECT_CALL(last, Append(&record)).
			WillOnce(Return(1));

	file.Add(record);
	EXPECT_EQ(5, CursorIndex());
	EXPECT_EQ(5, LastIndex());

	SetCursorNull();
	SetLastNull();
	SetDBNull();
}

TEST_F(HeapDBFileTest, ErrorHandling) {
	SetDBToheapdb();
	SetCursorIndex(5);
	SetLastIndex(5);
	SetCursor(cursor);
	SetLast(last);
	StrictMock<MockRecord> record;

	InSequence seq;
	EXPECT_CALL(last, Append(&record)).
			WillOnce(Return(0));
	EXPECT_CALL(mockFile, AddPage(&last, 5));
	EXPECT_CALL(last, EmptyItOut());
	EXPECT_CALL(last, Append(&record)).
			WillOnce(Return(0));

	EXPECT_THROW(file.Add(record), std::runtime_error);

	SetCursorNull();
	SetLastNull();
	SetDBNull();
}





