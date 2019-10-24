#include "SQLTest.h"
#include "SQLTest.h"
#include "Defs.h"
#include "ParseTree.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

TEST_F(SQLTest, TestforTableCreation) {
	char *relName[] = {"RelA"};

	stat.AddRel(relName[0],6001215);

	string cnf;
	cnf = "CREATE TABLE mytable (att1 INTEGER, att2 DOUBLE, att3 STRING) AS HEAP; ";

	vector<AttTypePair> atts;
	vector<string> order;
	string tableName;
	SQL test = SQL(stat);

	EXPECT_EQ(Create_Table, test.Parse(cnf));
	EXPECT_EQ(Heap, test.GetCreateTable(atts, order, tableName));

	ASSERT_EQ(3, atts.size());
	ASSERT_EQ(0, order.size());
	EXPECT_EQ(0, tableName.compare("mytable"));

	EXPECT_EQ(0, atts[0].Attribute().compare("att1"));
	EXPECT_EQ(0, atts[1].Attribute().compare("att2"));
	EXPECT_EQ(0, atts[2].Attribute().compare("att3"));

	EXPECT_EQ(Int, atts[0].GetType());
	EXPECT_EQ(Double, atts[1].GetType());
	EXPECT_EQ(String, atts[2].GetType());
}

TEST_F(SQLTest, TestforTableCreationIfExists) {
	char *relName[] = {"RelA"};

	stat.AddRel(relName[0],6001215);

	string cnf;
	cnf = "CREATE TABLE RelA (att1 INTEGER, att2 DOUBLE, att3 STRING) AS SORTED ON att1, att2;";

	vector<AttTypePair> atts;
	vector<string> order;
	string tableName;
	SQL test = SQL(stat);

	EXPECT_EQ(Create_Table, test.Parse(cnf));
	ASSERT_THROW(test.GetCreateTable(atts, order, tableName), invalid_argument);
}

TEST_F(SQLTest, TestforInsert) {
	char *relName[] = {"RelA"};

	stat.AddRel(relName[0],6001215);

	string str;
	str = "INSERT something.txt INTO RelA; ";

	string file;
	string table;
	SQL test = SQL(stat);

	EXPECT_EQ(Insert_Into, test.Parse(str));
	test.GetInsertInto(file, table);

	EXPECT_EQ(0, file.compare("something.txt"));
	EXPECT_EQ(0, table.compare("RelA"));
}

TEST_F(SQLTest, TestforInsertIfTableNotExists) {
	char *relName[] = {"RelA"};

	stat.AddRel(relName[0],6001215);

	string str;
	str = "INSERT something.txt INTO RelB; ";

	string file;
	string table;
	SQL test = SQL(stat);

	EXPECT_EQ(Insert_Into, test.Parse(str));
	ASSERT_THROW(test.GetInsertInto(file, table), invalid_argument);
}

TEST_F(SQLTest, TestforSetOutput) {
	char *relName[] = {"RelA"};
	stat.AddRel(relName[0],6001215);

	string str;
	str = "SET OUTPUT STDOUT; ";

	string file;
	SQL test = SQL(stat);

	EXPECT_EQ(Set_Output, test.Parse(str));
	EXPECT_EQ(Stdout, test.GetSetOutput(file));
}

bool SameComparisonOp(ComparisonOp *left, ComparisonOp *right) {
	if(left->code == right->code &&
			left->left->code == right->left->code &&
			strcmp(left->left->value, right->left->value) == 0 &&
			left->right->code == right->right->code &&
			strcmp(left->right->value, right->right->value) == 0) return true;
	return false;
}

bool SameOrList(OrList *left, OrList *right) {
	while(left != NULL || right != NULL) {
		if(!SameComparisonOp(left->left, right->left)) return false;
		left = left->rightOr;
		right = right->rightOr;
	}
	if(left != NULL || right != NULL) return false;
	return true;
}

bool SameAndList(AndList *left, AndList *right) {
	while(left != NULL || right != NULL) {
		if(!SameOrList(left->left, right->left)) return false;
		left = left->rightAnd;
		right = right->rightAnd;
	}
	if(left != NULL || right != NULL) return false;
	return true;
}

string OrListToString(OrList *list) {
	string temp = "";
	while (list != NULL) {
		temp.append(list->left->left->value);
		temp.append(" ");
		if(list->left->code == LESS_THAN) temp.append(" < ");
		if(list->left->code == GREATER_THAN) temp.append(" > ");
		if(list->left->code == EQUALS) temp.append(" = ");
		temp.append(" ");
		temp.append(list->left->right->value);
		temp.append(" ");
		if(list->rightOr != NULL) temp.append("OR ");
		list = list->rightOr;
	}
	return temp;
}

TEST_F(SQLTest, TestforParse) {
	char *relName[] = {"A", "B"};

	stat.AddRel(relName[0],6001215);
	stat.AddAtt(relName[0], "a1",3);
	stat.AddAtt(relName[0], "a2",11);
	stat.AddAtt(relName[0], "a3",7);
	stat.AddRel(relName[1],6001215);
	stat.AddAtt(relName[1], "b1",3);
	stat.AddAtt(relName[1], "b2",11);
	stat.AddAtt(relName[1], "b3",7);

	char *cnf = "(b1 = 5) AND (a1=b3) AND (a1 > 5 OR a2 = 9 OR a3 < 10)";

	yy_scan_string(cnf);
	yyparse();

	SelectMap selects;
	JoinMap joins;
	SQL test = SQL(stat, 2);

	test.ParseWhere(final, selects, joins);

	EXPECT_EQ(2, selects.size());
	EXPECT_EQ(2, joins.size());
	EXPECT_EQ(1, joins.at("A").size());
	EXPECT_EQ(1, joins.at("B").size());

	ASSERT_TRUE(selects.at("A") != NULL);
	ASSERT_TRUE(selects.at("B") != NULL);
	ASSERT_TRUE(joins.at("A").at("B") != NULL);
	ASSERT_TRUE(joins.at("B").at("A") != NULL);

	// Verify the AndLists
	EXPECT_TRUE(SameOrList(final->rightAnd->rightAnd->left, selects["A"]->left));
	EXPECT_TRUE(SameOrList(final->left, selects["B"]->left));
	EXPECT_TRUE(SameOrList(final->rightAnd->left, joins["A"]["B"]->left));
	EXPECT_TRUE(SameOrList(final->rightAnd->left, joins["B"]["A"]->left));
}

TEST_F(SQLTest, TestforParseLists) {
	char *relName[] = {"A", "B", "C"};

	stat.AddRel(relName[0],6001215);
	stat.AddAtt(relName[0], "a1",3);
	stat.AddAtt(relName[0], "a2",11);
	stat.AddAtt(relName[0], "a3",7);
	stat.AddRel(relName[1],6001215);
	stat.AddAtt(relName[1], "b1",3);
	stat.AddAtt(relName[1], "b2",11);
	stat.AddAtt(relName[1], "b3",7);
	stat.AddRel(relName[2],6001215);

	string sql = "SELECT A.a1, B.b1, B.b2 ";
	sql.append(" FROM A AS a, B AS b, C AS c ");
	sql.append(" WHERE (A.a1 = 5);");

	yysql_scan_string(sql.c_str());
	yysqlparse();

	vector<RelAliasPair> pair;
	SQL test = SQL(stat);
	test.ParseTableList(tables, pair);

	ASSERT_EQ(3, pair.size());
	EXPECT_TRUE(pair[0].Relation().compare("A") == 0);
	EXPECT_TRUE(pair[0].Alias().compare("a") == 0);
	EXPECT_TRUE(pair[1].Relation().compare("B") == 0);
	EXPECT_TRUE(pair[1].Alias().compare("b") == 0);
	EXPECT_TRUE(pair[2].Relation().compare("C") == 0);
	EXPECT_TRUE(pair[2].Alias().compare("c") == 0);
}

TEST_F(SQLTest, TestforDropTable) {
	char *relName[] = {"RelA"};

	stat.AddRel(relName[0],6001215);

	string str;
	str = "DROP TABLE RelA; ";

	SQL test = SQL(stat);

	EXPECT_EQ(Drop_Table, test.Parse(str));

	EXPECT_EQ(0, test.GetDropTable().compare("RelA"));
}

TEST_F(SQLTest, TestforDropTableIfNotExist) {
	char *relName[] = {"RelA"};

	stat.AddRel(relName[0],6001215);

	string str;
	str = "DROP TABLE RelB; ";

	string table;
	SQL test = SQL(stat);

	EXPECT_EQ(Drop_Table, test.Parse(str));
	ASSERT_THROW(test.GetDropTable(), invalid_argument);
}

