/*---
 * Title        : Project - 4.1,
 * Course       : DBI
 * Team Members : Priyam Saikia, Ruchika Mishra
*/
#ifndef INCLUDE_STATISTICS_H_
#define INCLUDE_STATISTICS_H_

#include <unordered_map>
#include <set>
#include <string>
#include <vector>
#include "Input.h"
#include "ParseTree.h"
#include "Statements.h"

struct StatData {
	double numTuples;
	std::unordered_map<std::string, int> atts;
	std::set<std::string> set;
};

class Statements;

class Statistics {
friend class StatisticsTest;

protected:
	std::unordered_map<std::string, StatData> &relations;
	std::unordered_map<std::string, StatData> myRelations;
	std::unordered_map<std::string, std::string> &lookup; // att -> rel mapping
	std::unordered_map<std::string, std::string> myLookup;

	Statistics(std::unordered_map<std::string, StatData> &_relations, std::unordered_map<std::string, std::string> &_lookup);
	virtual void Read(char *getLocation, Input &file);
	virtual void Write(char *getLocation, Input &file);
	virtual void UpdateRel(const char *relName, int numTuples);
	virtual bool IsLiteral(int code);
	virtual bool IsName(int code);
public:
	Statistics();
	Statistics(const Statistics &copyMe);
	virtual ~Statistics();
	virtual void RemoveRel(const char *relName);
	virtual void AddRel(const char *relName, double numTuples);
	virtual void Read(char *getLocation);
	virtual void Write(char *getLocation);
	virtual void CopyRel(const char *oldName, const char *newName);
	virtual void AddAtt(const char *relName, const char *attName, double numDistincts);
	virtual double NumTuples(const char *relName) const;
	virtual double GetUniqueVals(const char *relName, const char *attName) const;
	virtual void MergeSets(std::string rel1, std::string rel2);
	virtual bool JoinValidate(struct AndList *parseTree, const char **relNames, int numToJoin);
	virtual bool ConfirmAction(struct AndList *parseTree, const char **relNames, int numToJoin);
	virtual bool GetCombinedSets(const char **relNames, int numToJoin);
	virtual bool ParseOperand(std::string operand, std::vector<std::string> &out) const;
	virtual std::string RelLookup(std::string att) const;
	virtual std::set<std::string> GetSet(std::string rel);
	virtual void MakeStatements(ComparisonOp op, std::vector<Statements*>& expressions, std::set<std::string>& relations);
 	virtual double Join(OrList* orList, std::set<std::string>& relations);
 	virtual void CombineStatementss(std::vector<Statements*>& expressions);
 	virtual double ComputeNumTuples(std::vector<Statements*>& expressions);
 	virtual double Simplify(struct AndList *parseTree, const char *relNames[], int numToJoin);
 	virtual void  Apply(struct AndList *parseTree, const char *relNames[], int numToJoin);
	virtual double Estimate(struct AndList *parseTree, const char **relNames, int numToJoin);
	virtual int RelationSize();
};

#endif /* INCLUDE_STATISTICS_H_ */
