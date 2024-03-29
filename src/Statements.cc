/*---
 * Title        : Project - 4.1,
 * Course       : DBI
 * Team Members : Priyam Saikia, Ruchika Mishra
*/
#include "Statements.h"
#include <iostream>

using namespace std;

Statements::Statements(): stat(stat), type(), op() {}
Statements::Statements(StatementsType _type, Statistics &_stat, int _op): stat(_stat),
		type(_type), op(_op) {}

Statements::~Statements() {}

BinaryStatements::BinaryStatements() {}

BinaryStatements::BinaryStatements(Statistics &_stat, string _rel1, string _att1, string _rel2,
		string _att2, int _op): Statements(BINARY, _stat, _op), rel1(_rel1), att1(_att1),
				rel2(_rel2), att2(_att2) {}

BinaryStatements::~BinaryStatements() {}

bool BinaryStatements::Combine(Statements &e) {
	switch(e.type) {
		case BINARY:
		{
			BinaryStatements *temp = dynamic_cast<BinaryStatements*>(&e);
			BinaryStatements &be = *temp;

			// Same relations/attributes in the same position
			if(rel1.compare(be.rel1) == 0 && att1.compare(be.att1) == 0 &&
					rel2.compare(be.rel2) == 0 && att2.compare(be.att2) == 0)
			{
				if(op == be.op) return true;
				else return false;
			}
			// Same relations/attributes in different positions
			else if(rel1.compare(be.rel2) == 0 && att1.compare(be.att2) == 0 &&
					rel2.compare(be.rel1) == 0 && att2.compare(be.att1) == 0 ) {
				if( (op == EQUALS && be.op == EQUALS) ||
						(op == LESS_THAN && be.op == GREATER_THAN) ||
						(op == GREATER_THAN && be.op == LESS_THAN) ) return true;
				else return false;
			}
			else return false;
			break;
		}
		case UNARY:
			return false;
			break;
		default:
			return false;
			break;
	}

	return false;
}

double BinaryStatements::Tuples() {
	double numerator = stat.NumTuples(rel1.c_str()) * stat.NumTuples(rel2.c_str());
	return numerator/Denominator();
}

double BinaryStatements::Numerator() {
	return stat.NumTuples(rel1.c_str());
}

double BinaryStatements::Denominator() {
	if(op == EQUALS) return stat.GetUniqueVals(rel1.c_str(), att1.c_str());
	else return GT_LT_DENOMINATOR;
}

UnaryStatements::UnaryStatements() {}

UnaryStatements::UnaryStatements(Statistics &_stat, string _rel, string _att, string lit, int _op):
		Statements(UNARY, _stat, _op), rel(_rel), att(_att) {
	literals.insert(lit);
}

UnaryStatements::~UnaryStatements() {}

bool UnaryStatements::Combine(Statements &e) {
	switch(e.type) {
		case BINARY:
			return false;
			break;
		case UNARY:
		{
			UnaryStatements *temp = dynamic_cast<UnaryStatements*>(&e);
			UnaryStatements &ue = *temp;
			if(op == ue.op && (rel.compare(ue.rel) == 0 && att.compare(ue.att) == 0)) {
				// Now, time to eliminate duplicates
				for(auto it = ue.literals.begin(); it != ue.literals.end(); ++it) {
					literals.insert(*it);
				}
				return true;
			}
			else return false;
			break;
		}
		default:
			return false;
			break;
	}
	return false;
}

double UnaryStatements::Tuples() {
	if(op == EQUALS) {
		double numerator = stat.NumTuples(rel.c_str()) * literals.size();
		return numerator/Denominator();
	}
	else return stat.NumTuples(rel.c_str())/ GT_LT_DENOMINATOR;
}

double UnaryStatements::Numerator() {
	return stat.NumTuples(rel.c_str());
}

double UnaryStatements::Denominator() {
	if(op == EQUALS) return stat.GetUniqueVals(rel.c_str(), att.c_str());
	else return GT_LT_DENOMINATOR;
}

int UnaryStatements::Count() {
	return literals.size();
}
