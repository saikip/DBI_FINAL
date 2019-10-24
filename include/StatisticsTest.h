/*---
 * Title        : Project - 4.1,
 * Course       : DBI
 * Team Members : Priyam Saikia, Ruchika Mishra
*/
#ifndef INCLUDE_STATISTICSTEST_H_
#define INCLUDE_STATISTICSTEST_H_

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "Statistics.h"
#include "MockClasses.h"
#include "ParseTree.h"

extern "C" struct YY_BUFFER_STATE *yy_scan_string(const char*);
extern "C" struct YY_BUFFER_STATE *yysql_scan_string(const char*);
extern "C" int yyparse(void);
extern "C" int yysqlparse(void);
extern struct AndList *final;
extern struct NameList *attsToSelect;

using ::testing::_;
using ::testing::Return;
using ::testing::InSequence;
using ::testing::Sequence;
using ::testing::StrictMock;
using ::testing::AtMost;
using ::testing::AtLeast;
using ::testing::ByRef;
using ::testing::Pointee;
using ::testing::NotNull;
using ::testing::SetArgPointee;
using ::testing::DoAll;
using ::testing::Eq;
using ::testing::Ref;

class StatisticsTest: public::testing::Test {
public:
	std::unordered_map<std::string, StatData> map;
	std::unordered_map<std::string, std::string> lookup;
	Statistics stat = Statistics(map, lookup);

	void Read(char *name, Input &file) { stat.Read(name, file); }

	void Write(char *name, Input &file) { stat.Write(name, file); }

	void UpdateRel(char *relName, int numTuples) { stat.UpdateRel(relName, numTuples); }

	string GetRel(UnaryStatements& ue) { return ue.rel; }
	string GetAtt(UnaryStatements& ue) { return ue.att; }
	set<string> GetLits(UnaryStatements& ue) { return ue.literals; }
	string GetRel1(BinaryStatements &be) { return be.rel1; }
	string GetRel2(BinaryStatements &be) { return be.rel2; }
	string GetAtt1(BinaryStatements &be) { return be.att1; }
	string GetAtt2(BinaryStatements &be) { return be.att2; }
};

#endif /* INCLUDE_STATISTICSTEST_H_ */