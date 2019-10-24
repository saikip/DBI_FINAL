#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "MockClasses.h"
#include "StatisticsTest.h"

TEST_F(StatisticsTest, TestforAddRel) {
	stat.AddRel("test1", 5);
	stat.AddRel("test2", 15);
	stat.AddRel("test3", 25);
	stat.AddRel("test4", 13);
	stat.AddRel("test5", 69);

	EXPECT_EQ(5, map["test1"].numTuples);
	EXPECT_EQ(15, map["test2"].numTuples);
	EXPECT_EQ(25, map["test3"].numTuples);
	EXPECT_EQ(13, map["test4"].numTuples);
	EXPECT_EQ(69, map["test5"].numTuples);
}
TEST_F(StatisticsTest, TestforAddAtt) {
	// Create relations
	map["test1"];
	map["test2"];
	map["test3"];
	map["test4"];

	stat.AddAtt("test1", "bob", 5);
	stat.AddAtt("test2", "alice", 15);
	stat.AddAtt("test3", "great", 25);
	stat.AddAtt("test4", "scott", 13);
	stat.AddAtt("test4", "batman", 69);

	EXPECT_EQ(map["test1"].atts["bob"], 5);
	EXPECT_EQ(map["test2"].atts["alice"], 15);
	EXPECT_EQ(map["test3"].atts["great"], 25);
	EXPECT_EQ(map["test4"].atts["scott"], 13);
	EXPECT_EQ(map["test4"].atts["batman"], 69);
}
TEST_F(StatisticsTest, TestforCopyRel) {
	map["test"].numTuples = 5;
	map["test"].atts["some"] = 10;
	map["test"].atts["cool"] = 13;
	map["test"].atts["quote"] = 100;

	stat.CopyRel("test", "new");
	EXPECT_EQ(5, map["new"].numTuples);
	EXPECT_EQ(10, map["new"].atts["some"]);
	EXPECT_EQ(13, map["new"].atts["cool"]);
	EXPECT_EQ(100, map["new"].atts["quote"]);
}
TEST_F(StatisticsTest, TestforRead) {
	StrictMock<MockInput> file;
	InSequence seq;
	EXPECT_CALL(file, Open("file.txt"));
	EXPECT_CALL(file, ReadLine(NotNull())).
			WillOnce(DoAll(SetArgPointee<0>("Relations"), Return(true)));
	EXPECT_CALL(file, ReadLine(NotNull())).
			WillOnce(DoAll(SetArgPointee<0>("Attributes"), Return(true)));
	EXPECT_CALL(file, ReadLine(NotNull())).
			WillOnce(Return(false));
	EXPECT_CALL(file, Close()).
			WillOnce(Return(true));
	Read("file.txt", file);
}
TEST_F(StatisticsTest, TestforApply) {
	StrictMock<MockStatements> mock;
	vector<Statements*> exps;
	exps.push_back(&mock);
	stat.CombineStatementss(exps);
	EXPECT_EQ(1, exps.size());
}
TEST_F(StatisticsTest, TestforStatements) {
	StrictMock<MockStatements> mock1;
	StrictMock<MockStatements> mock2;
	StrictMock<MockStatements> mock3;
	StrictMock<MockStatements> mock4;
	StrictMock<MockStatements> mock5;
	vector<Statements*> exps;
	exps.push_back(&mock1);
	exps.push_back(&mock2);
	exps.push_back(&mock3);
	exps.push_back(&mock4);
	exps.push_back(&mock5);

	Sequence s;
	EXPECT_CALL(mock1, Combine(_)).
		WillRepeatedly(Return(false));
	EXPECT_CALL(mock2, Combine(_)).
		InSequence(s).
		WillOnce(Return(false));
	EXPECT_CALL(mock2, Combine(_)).
		InSequence(s).
		WillOnce(Return(true));
	EXPECT_CALL(mock2, Combine(_)).
		InSequence(s).
		WillOnce(Return(false));
	EXPECT_CALL(mock3, Combine(_)).
		WillRepeatedly(Return(false));
	EXPECT_CALL(mock4, Combine(_)).
		WillRepeatedly(Return(false));
	EXPECT_CALL(mock5, Combine(_)).
		WillRepeatedly(Return(false));

	stat.CombineStatementss(exps);	
	EXPECT_EQ(4, exps.size());
}
TEST_F(StatisticsTest, TestforUpdateRel) {
	UpdateRel("test1", 5);
	UpdateRel("test2", 15);
	UpdateRel("test3", 25);
	UpdateRel("test4", 13);
	UpdateRel("test5", 69);

	EXPECT_EQ(map["test1"].numTuples, 5);
	EXPECT_EQ(map["test2"].numTuples, 15);
	EXPECT_EQ(map["test3"].numTuples, 25);
	EXPECT_EQ(map["test4"].numTuples, 13);
	EXPECT_EQ(map["test5"].numTuples, 69);
}
TEST_F(StatisticsTest, TestforRelNames) {
	stat.AddRel("rel1", 0);
	stat.AddRel("rel2", 0);
	stat.AddRel("rel3", 0);
	stat.AddRel("rel4", 0);
	stat.AddRel("rel5", 0);

	stat.MergeSets("rel1", "rel2");
	stat.MergeSets("rel1", "rel3");
	stat.MergeSets("rel1", "rel4");

	const char *relNames[] = { "rel1", "rel2", "rel3", "rel5" };

	EXPECT_EQ(false, stat.GetCombinedSets(relNames, 4));
}
TEST_F(StatisticsTest, TestforParseOp) {
	string operand = "nothing";
	stat.AddRel("rel", 3);
	stat.AddAtt("rel", "att", 0);
	vector<string> out;

	EXPECT_EQ(false, stat.ParseOperand(operand, out));
}
TEST_F(StatisticsTest, TestforFinalTuplesNum) {
	StrictMock<MockStatements> mock;
	vector<Statements*> exps;
	exps.push_back(&mock);

	EXPECT_CALL(mock, Tuples()).
		WillOnce(Return(10.0));

	EXPECT_EQ(10.0, stat.ComputeNumTuples(exps));
}
TEST_F(StatisticsTest, TestforCopyConstructor) {
	// Add Relations
	map["rel1"].numTuples = 1;
	map["rel2"].numTuples = 2;
	map["rel3"].numTuples = 3;
	map["rel4"].numTuples = 4;

	// Add Attributes
	map["rel1"].atts["att1"] = 1;
	map["rel1"].atts["att2"] = 2;
	map["rel2"].atts["att3"] = 3;
	map["rel2"].atts["att4"] = 4;
	map["rel3"].atts["att5"] = 5;
	map["rel3"].atts["att6"] = 6;
	map["rel3"].atts["att7"] = 7;
	map["rel3"].atts["att8"] = 8;
	map["rel3"].atts["att9"] = 9;

	Statistics copy = Statistics(stat);

	EXPECT_EQ(copy.NumTuples("rel1"), 1);
	EXPECT_EQ(copy.NumTuples("rel2"), 2);
	EXPECT_EQ(copy.NumTuples("rel3"), 3);
	EXPECT_EQ(copy.NumTuples("rel4"), 4);

	EXPECT_EQ(copy.GetUniqueVals("rel1", "att1"), 1);
	EXPECT_EQ(copy.GetUniqueVals("rel1", "att2"), 2);
	EXPECT_EQ(copy.GetUniqueVals("rel2", "att3"), 3);
	EXPECT_EQ(copy.GetUniqueVals("rel2", "att4"), 4);
	EXPECT_EQ(copy.GetUniqueVals("rel3", "att5"), 5);
	EXPECT_EQ(copy.GetUniqueVals("rel3", "att6"), 6);
	EXPECT_EQ(copy.GetUniqueVals("rel3", "att7"), 7);
	EXPECT_EQ(copy.GetUniqueVals("rel3", "att8"), 8);
	EXPECT_EQ(copy.GetUniqueVals("rel3", "att9"), 9);
}
TEST_F(StatisticsTest, TestforparsingUnary) {
	Operand left {INT, "4"};
	Operand right {NAME, "attribute"};
	ComparisonOp op {LESS_THAN, &left, &right};
	vector<Statements*> expressions;
	set<string> relations;

	stat.AddRel("relation", 100);
	stat.AddAtt("relation", "attribute", 10);

	stat.MakeStatements(op, expressions, relations);
	ASSERT_EQ(1, expressions.size());
	EXPECT_EQ(UNARY, expressions[0]->type);
	EXPECT_EQ(LESS_THAN, expressions[0]->op);
	UnaryStatements* ue = dynamic_cast<UnaryStatements*> (expressions[0]);
	EXPECT_STREQ("relation", GetRel(*ue).c_str());
	EXPECT_STREQ("attribute", GetAtt(*ue).c_str());
	ASSERT_EQ(1, GetLits(*ue).size());
	auto it = GetLits(*ue).begin();
	EXPECT_STREQ("4", it->c_str());
	ASSERT_EQ(1, relations.size());
	it = relations.begin();
	ASSERT_EQ("relation", *it);
}

TEST_F(StatisticsTest, TestforparsingBinary) {
	Operand left {NAME, "attLeft"};
	Operand right {NAME, "attRight"};
	ComparisonOp op {EQUALS, &left, &right};
	vector<Statements*> expressions;
	set<string> relations;

	stat.AddRel("relLeft", 100);
	stat.AddAtt("relLeft", "attLeft", 10);
	stat.AddRel("relRight", 100);
	stat.AddAtt("relRight", "attRight", 10);

	stat.MakeStatements(op, expressions, relations);
	ASSERT_EQ(1, expressions.size());
	EXPECT_EQ(BINARY, expressions[0]->type);
	EXPECT_EQ(EQUALS, expressions[0]->op);
	BinaryStatements* be = dynamic_cast<BinaryStatements*> (expressions[0]);
	EXPECT_STREQ("relLeft", GetRel1(*be).c_str());
	EXPECT_STREQ("attLeft", GetAtt1(*be).c_str());
	EXPECT_STREQ("relRight", GetRel2(*be).c_str());
	EXPECT_STREQ("attRight", GetAtt2(*be).c_str());
	ASSERT_EQ(2, relations.size());
	auto it = relations.begin();
	ASSERT_EQ("relLeft", *it);
	it++;
	ASSERT_EQ("relRight", *it);
}

TEST_F(StatisticsTest, TestforEstimateFunction) {
	const char *relName[] = {"supplier","partsupp"};


	stat.AddRel(relName[0],10000);
	stat.AddAtt(relName[0], "s_suppkey",10000);

	stat.AddRel(relName[1],800000);
	stat.AddAtt(relName[1], "ps_suppkey", 10000);

	char *cnf = "(s_suppkey = ps_suppkey)";

	yy_scan_string(cnf);
	yyparse();
	double result = stat.Estimate(final, relName, 2);
	EXPECT_EQ(800000, result);
	stat.Apply(final, relName, 2);

	//reload the statistics object from file
	cnf = "(s_suppkey>1000)";
	yy_scan_string(cnf);
	yyparse();
	double dummy = stat.Estimate(final, relName, 2);
	EXPECT_NEAR(dummy*3.0, result, .1);
}

TEST_F(StatisticsTest, TestforCombinedSets) {
	stat.AddRel("test1", 5);
	stat.AddRel("test2", 15);

	stat.MergeSets("test1", "test2");

	set<string> set1 = stat.GetSet("test1"); // test1, test2
	set<string> set2 = stat.GetSet("test2"); // test3, test4, test5

	EXPECT_EQ(1, set1.count("test1"));
	EXPECT_EQ(1, set1.count("test2"));
	EXPECT_EQ(1, set2.count("test1"));
	EXPECT_EQ(1, set2.count("test2"));

	stat.MergeSets("test1", "test2");
	set<string> set3 = stat.GetSet("test1"); // test1, test2
	set<string> set4 = stat.GetSet("test2"); // test3, test4, test5
	EXPECT_EQ(1, set3.count("test1"));
	EXPECT_EQ(1, set3.count("test2"));
	EXPECT_EQ(1, set4.count("test1"));
	EXPECT_EQ(1, set4.count("test2"));
}
