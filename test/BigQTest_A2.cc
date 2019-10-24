#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "MockClasses.h"
#include "BigQ.h"
#include "BigQTest.h"

TEST_F(BigQTest, TestforAddRecord) {
	MockRecord mRec;
	Record *r = &mRec;
	SetRunSizeInBytes(10);
	SetCurrRunSizeInBytes(0);

	EXPECT_CALL(mRec, Size()).
		WillRepeatedly(Return(3));

	ASSERT_EQ(0, run.size());
	ASSERT_EQ(true, AddRecord(r));
	ASSERT_EQ(1, run.size());
	ASSERT_EQ(3, GetCurrRunSizeInBytes());
	ASSERT_EQ(&mRec, run[0]);
}

TEST_F(BigQTest, TestForMultipleRecords) {
	MockRecord t1;
	MockRecord t2;
	MockRecord t3;
	MockRecord t4;
	MockRecord t5;

	Record *r1 = &t1;
	Record *r2 = &t2;
	Record *r3 = &t3;
	Record *r4 = &t4;
	Record *r5 = &t5;

	SetRunSizeInBytes(10);
	SetCurrRunSizeInBytes(0);

	EXPECT_CALL(t1, Size()).
		WillRepeatedly(Return(1));
	EXPECT_CALL(t2, Size()).
		WillRepeatedly(Return(2));
	EXPECT_CALL(t3, Size()).
		WillRepeatedly(Return(3));
	EXPECT_CALL(t4, Size()).
		WillRepeatedly(Return(4));
	EXPECT_CALL(t5, Size()).
		WillRepeatedly(Return(5));

	ASSERT_EQ(true, AddRecord(r1));
	ASSERT_EQ(1, run.size());
	ASSERT_EQ(&t1, run[0]);
	ASSERT_EQ(1, GetCurrRunSizeInBytes());
	ASSERT_EQ(true, AddRecord(r2));
	ASSERT_EQ(2, run.size());
	ASSERT_EQ(&t2, run[1]);
	ASSERT_EQ(3, GetCurrRunSizeInBytes());
	ASSERT_EQ(true, AddRecord(r3));
	ASSERT_EQ(3, run.size());
	ASSERT_EQ(&t3, run[2]);
	ASSERT_EQ(6, GetCurrRunSizeInBytes());
	ASSERT_EQ(true, AddRecord(r4));
	ASSERT_EQ(4, run.size());
	ASSERT_EQ(&t4, run[3]);
	ASSERT_EQ(10, GetCurrRunSizeInBytes());
	ASSERT_EQ(false, AddRecord(r5));
	ASSERT_EQ(4, run.size());
	ASSERT_EQ(&t4, run[3]);
	ASSERT_EQ(10, GetCurrRunSizeInBytes());
}

TEST_F(BigQTest, TestLargeEntry) {
	MockRecord temp;
		Record *r = &temp;
	SetRunSizeInBytes(10);
	SetCurrRunSizeInBytes(0);

	EXPECT_CALL(temp, Size()).
		WillRepeatedly(Return(11));

	EXPECT_THROW(AddRecord(r), std::runtime_error);
	ASSERT_EQ(0, run.size());
	ASSERT_EQ(0, GetCurrRunSizeInBytes());
}
TEST_F(BigQTest, PageTest) {
	Record *temp[5];
	Record **heads = temp;
	off_t temp2[5];
	off_t *runIndex = temp2;
	Page *temp3[5];
	Page **pages = temp3;

	int runsLeft = 3;

	StrictMock<MockRecord> head0;
	StrictMock<MockRecord> head1;
	StrictMock<MockRecord> head2;
	StrictMock<MockRecord> head3;
	StrictMock<MockRecord> head4;

	heads[0] = &head0;
	heads[1] = &head1;
	heads[2] = &head2;
	heads[3] = &head3;
	heads[4] = &head4;

	runPos.push_back(0);
	runPos.push_back(2);
	runPos.push_back(5);
	runPos.push_back(7);
	runPos.push_back(9);
	runPos.push_back(10);
	runIndex[3] = 7;

	StrictMock<MockPage> minPage;
	pages[0] = NULL;
	pages[1] = NULL;
	pages[2] = NULL;
	pages[3] = &minPage;
	pages[4] = NULL;

	
	{
		InSequence seq;
		EXPECT_CALL(minPage, GetFirst(heads[3])).
			WillOnce(Return(0));

		EXPECT_CALL(file, GetPage(pages[3], 8));

		EXPECT_CALL(minPage, GetFirst(heads[3])).
			WillOnce(Return(1));
	}
	

	GetNextRecord(3, heads, runIndex, pages, runsLeft);
	ASSERT_EQ(3, runsLeft);
	ASSERT_EQ(8, runIndex[3]);
}

TEST_F(BigQTest, TestRunPages) {
	Sequence s1;
	EXPECT_CALL(page, Append(NotNull())).
			Times(5).
			InSequence(s1).
			WillRepeatedly(Return(1));
	EXPECT_CALL(page, Append(NotNull())).
			InSequence(s1).
			WillOnce(Return(0));
	EXPECT_CALL(page, Append(NotNull())).
			Times(5).
			InSequence(s1).
			WillRepeatedly(Return(1));
	EXPECT_CALL(page, Append(NotNull())).
			InSequence(s1).
			WillOnce(Return(0));
	EXPECT_CALL(page, Append(NotNull())).
			Times(5).
			InSequence(s1).
			WillRepeatedly(Return(1));

	EXPECT_CALL(page, EmptyItOut()).
			Times(AtLeast(3));

	EXPECT_CALL(file, AddPage(&page, 10));
	EXPECT_CALL(file, AddPage(&page, 11));
	EXPECT_CALL(file, AddPage(&page, 12));

	off_t totalPageCount = 10;
	for(int i = 0; i < 15; i++) {
		run.push_back(new Record());
	}

	ProcessPages(totalPageCount);
	EXPECT_EQ(13, totalPageCount);
	EXPECT_EQ(0, run.size());
}

TEST_F(BigQTest, TestPagesPartial) {
	Sequence s1;
	EXPECT_CALL(page, Append(NotNull())).
			Times(2).
			InSequence(s1).
			WillRepeatedly(Return(1));
	EXPECT_CALL(page, Append(NotNull())).
			InSequence(s1).
			WillOnce(Return(0));
	EXPECT_CALL(page, Append(NotNull())).
			Times(2).
			InSequence(s1).
			WillRepeatedly(Return(1));

	EXPECT_CALL(page, EmptyItOut()).
			Times(AtLeast(3));

	EXPECT_CALL(file, AddPage(&page, 10));
	EXPECT_CALL(file, AddPage(&page, 11));

	off_t totalPageCount = 10;
	for(int i = 0; i < 4; i++) {
		run.push_back(new Record());
	}

	ProcessPages(totalPageCount);
	EXPECT_EQ(12, totalPageCount);
	EXPECT_EQ(0, run.size());
}

TEST_F(BigQTest, TestTPMMS1) {
	Record *temp[5];
	Record **head = temp;
	Record rec;
	for(int i = 0; i < 5; i++) { temp[i] = &rec; }

	Sequence s1;
	EXPECT_CALL(comp, Compare(NotNull(), NotNull(), &order)).
			Times(1).
			InSequence(s1).
			WillRepeatedly(Return(0));
	EXPECT_CALL(comp, Compare(NotNull(), NotNull(), &order)).
			InSequence(s1).
			WillOnce(Return(1));
	EXPECT_CALL(comp, Compare(NotNull(), NotNull(), &order)).
			Times(2).
			InSequence(s1).
			WillRepeatedly(Return(-2));


	EXPECT_EQ(2, FindMin(5,head));
}

TEST_F(BigQTest, TestTPMMS2) {
	Record *temp[5];
	Record **head = temp;
	Record rec;
	for(int i = 0; i < 5; i++) { temp[i] = NULL; }
	temp[2] = &rec;

	EXPECT_EQ(2, FindMin(5,head));
}

TEST_F(BigQTest, TestforGetNext) {
	Record *temp[5];
	Record **heads = temp;
	off_t temp2[5];
	off_t *runIndex = temp2;
	Page *temp3[5];
	Page **pages = temp3;
	MockRecord mockNextMin;

	int runsLeft = 3;

	StrictMock<MockRecord> head0;
	StrictMock<MockRecord> head1;
	StrictMock<MockRecord> head2;
	StrictMock<MockRecord> head3;
	StrictMock<MockRecord> head4;

	heads[0] = &head0;
	heads[1] = &head1;
	heads[2] = &head2;
	heads[3] = &head3;
	heads[4] = &head4;

	StrictMock<MockPage> minPage;

	pages[0] = NULL;
	pages[1] = NULL;
	pages[2] = NULL;
	pages[3] = &minPage;
	pages[4] = NULL;

	EXPECT_CALL(minPage, GetFirst(&head3)).
			WillOnce(Return(1));
	
	GetNextRecord(3, heads, runIndex, pages, runsLeft);
	EXPECT_EQ(3, runsLeft);
	EXPECT_EQ(&head3, heads[3]);
}



TEST_F(BigQTest, TestErrorHandling) {
	Sequence s1;
	EXPECT_CALL(page, Append(NotNull())).
			Times(2).
			InSequence(s1).
			WillRepeatedly(Return(1));
	EXPECT_CALL(page, Append(NotNull())).
			InSequence(s1).
			WillOnce(Return(0));
	EXPECT_CALL(page, Append(NotNull())).
			InSequence(s1).
			WillOnce(Return(0));

	EXPECT_CALL(page, EmptyItOut()).
			Times(AtLeast(1));

	EXPECT_CALL(file, AddPage(&page, 10));

	off_t totalPageCount = 10;
	for(int i = 0; i < 4; i++) {
		run.push_back(new Record());
	}

	EXPECT_THROW(ProcessPages(totalPageCount), runtime_error);
}


