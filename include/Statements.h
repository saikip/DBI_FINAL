/*---
 * Title        : Project - 4.1,
 * Course       : DBI
 * Team Members : Priyam Saikia, Ruchika Mishra
*/
#ifndef INCLUDE_STATEMENTS_H_
#define INCLUDE_STATEMENTS_H_

#include "Statistics.h"
#include "ParseTree.h"
#include <set>

#define GT_LT_DENOMINATOR 3;

class Statistics;

enum StatementsType { BINARY, UNARY };

class Statements {
protected:
	Statistics &stat;

public:
	StatementsType type;
	int op; 

	Statements();
	Statements(StatementsType _type, Statistics &_stat, int _op);
	virtual ~Statements();
	virtual bool Combine(Statements &e){return false;}
	virtual double Tuples() {return 0;}
	virtual double Numerator() {return 0;}
	virtual double Denominator() {return 0;}
};

class BinaryStatements: public Statements {
friend class StatisticsTest;

protected:
	std::string rel1, rel2;
	std::string att1, att2;
public:
	BinaryStatements();
	BinaryStatements(Statistics &_stat, std::string _rel1, std::string _att1, std::string _rel2, std::string _att2, int _op);
	virtual ~BinaryStatements();
	virtual bool Combine(Statements &e);
	virtual double Tuples();
	virtual double Numerator();
	virtual double Denominator();
};

class UnaryStatements: public Statements {
friend class StatisticsTest;

protected:
	std::string rel;
	std::string att;
	std::set<std::string> literals;

public:
	UnaryStatements();
	UnaryStatements(Statistics &_stat, std::string _rel, std::string _att, std::string lit, int _op);
	virtual ~UnaryStatements();
	virtual bool Combine(Statements &e);
	virtual double Tuples();
	virtual double Numerator();
	virtual double Denominator();
	virtual int Count();
};

#endif /* INCLUDE_STATEMENTS_H_ */
