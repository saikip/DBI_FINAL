/*---
 * Title        : Project - 4.1,
 * Course       : DBI
 * Team Members : Priyam Saikia, Ruchika Mishra
*/
using namespace std;

#include "Statistics.h"
#include <iostream>

Statistics::Statistics(): relations(myRelations), lookup(myLookup) {}

Statistics::Statistics(std::unordered_map<std::string, StatData> &_relations,
		std::unordered_map<std::string, std::string> &_lookup):
				relations(_relations), lookup(_lookup) {
}

Statistics::Statistics(const Statistics &copyMe): relations(myRelations), lookup(myLookup) {
	string rel;
	string att;
	int val;
	StatData temp;

	for(auto it = copyMe.relations.begin(); it != copyMe.relations.end(); ++it) {
		rel = it->first;
		temp = it->second;
		relations[rel].numTuples = temp.numTuples; 
		for(auto a_it = temp.atts.begin(); a_it != temp.atts.end(); ++a_it) {
			att = a_it->first;
			relations[rel].atts[a_it->first] =  a_it->second;
		}
		for(auto s_it = temp.set.begin(); s_it != temp.set.end(); ++s_it) {
			relations[rel].set.insert(*s_it);
		}
	}

	for(auto it = copyMe.lookup.begin(); it != copyMe.lookup.end(); ++it) {
		lookup[it->first] = it->second;
	}
}

Statistics::~Statistics() {
	// TODO Auto-generated destructor stub
}

void Statistics::RemoveRel(const char* relName) {
	relations.erase(string(relName));
	lookup.erase(string(relName));
}

void Statistics::AddRel(const char *relName, double numTuples) {
	try {
		set<string> &temp = relations.at(relName).set;

		for(auto it = temp.begin(); it != temp.end(); ++it) {
			UpdateRel((*it).c_str(), numTuples);
		}
	}
	catch(out_of_range &e) {
		relations[relName].numTuples = numTuples;
		relations[relName].set.insert(relName);
	}
}

void Statistics::UpdateRel(const char *relName, int numTuples) {
	relations[relName].numTuples = numTuples;
}

void Statistics::Read(char *getLocation) {
	Input file;
	Read(getLocation, file);
}

void Statistics::Read(char *getLocation, Input &file) {
	string line;
	string rel;
	string val;
	int equals;

	file.Open(getLocation);
	file.ReadLine(&line);
	while(file.ReadLine(&line) == true && line.compare("Attributes") != 0) {
		equals = line.find_first_of("=");
		if(equals != string::npos) {
			rel = line.substr(0, equals);
			val = line.substr(equals + 1, line.length() + 1);
			relations[rel].numTuples = stoi(val);
			relations[rel].set.insert(rel);
		}
	}

	string att;
	int dot;
	while(file.ReadLine(&line) == true) {
		equals = line.find_first_of("=");
		dot = line.find_first_of(".");
		if(equals != string::npos && dot != string::npos) { 
			rel = line.substr(0, dot);
			att = line.substr(dot+1, equals - dot - 1);
			val = line.substr(equals + 1, line.length() + 1);
			relations.at(rel).atts[att] = stoi(val);
			lookup[att] = rel;
		}
	}

	file.Close();
}

void Statistics::Write(char *getLocation) {
	Input file;
	Write(getLocation, file);
}

void Statistics::Write(char *getLocation, Input &file) {
	file.Open(getLocation);

	string rel;
	string val;
	file.Append("*******************************************");
	file.Append("\n Relations: \n");
	for(auto it = relations.begin(); it != relations.end(); ++it) {
		rel = it->first;
		val = to_string(it->second.numTuples);
		file.Append(rel + "=" + val + "\n");
	}

	string att;
	file.Append("Attributes: \n");
	for(auto it = relations.begin(); it != relations.end(); ++it) {
		rel = it->first;
		val = to_string(it->second.numTuples);
		for(auto a_it = relations.at(rel).atts.begin(); a_it != relations.at(rel).atts.end(); ++a_it) {
			att = a_it->first;
			val = to_string(a_it->second);
			file.Append(rel + "." + att + "=" + val + "\n");
		}
	}
	file.Append("*******************************************");
	file.Close();
}

void Statistics::CopyRel(const char *oldName, const char *newName) {
	relations[newName].numTuples = relations.at(oldName).numTuples;

	for(auto it = relations.at(oldName).atts.begin(); it != relations.at(oldName).atts.end(); ++it) {
		relations[newName].atts[it->first] = it->second;
	}

	relations[newName].set.insert(newName);
}

void Statistics::AddAtt(const char *relName, const char *attName, double numDistincts) {
	relations.at(relName).atts[attName] = numDistincts;

	try {
		lookup.at(attName);
	}
	catch(out_of_range &e) {
		lookup[attName] = relName;
	}
}

double Statistics::NumTuples(const char *relName) const {
	try {
		return relations.at(relName).numTuples;
	}
	catch(out_of_range &e) {
		return -1;
	}
}

double Statistics::GetUniqueVals(const char *relName, const char *attName) const {
	try {
		if(relations.at(relName).atts.at(attName) == -1) return relations.at(relName).numTuples;
		else return relations.at(relName).atts.at(attName);
	}
	catch(out_of_range &e) {
		return -1;
	}
}

double Statistics::Simplify(struct AndList *parseTree, const char *relNames[], int numToJoin) {
	if(!JoinValidate(parseTree, relNames, numToJoin)) {
		throw std::runtime_error("Statistics::Simplify: cannot perform join");
	}
	if(parseTree->left == NULL && numToJoin == 1) {
		return NumTuples(relNames[0]);
	}
	set<string> relations;
	AndList* curr = parseTree;
	double numTuples = 0;
	while(curr != NULL) {
		relations.clear();
		numTuples = Join(curr->left, relations);

		auto it = relations.begin();
		auto first = *it;
		++it;
		for(; it != relations.end(); ++it) {
			MergeSets(first, *it);
		}
		AddRel(first.c_str(), numTuples);
		curr = curr->rightAnd;
	}
	return numTuples;
}

void  Statistics::Apply(struct AndList *parseTree, const char *relNames[], int numToJoin)
{
	Simplify(parseTree, relNames, numToJoin);
}

double Statistics::Estimate(struct AndList *parseTree, const char **relNames, int numToJoin)
{
	Statistics copy(*this);
	return copy.Simplify(parseTree, relNames, numToJoin);
}

void Statistics::MergeSets(std::string rel1, std::string rel2) {
	set<string> temp1 = GetSet(rel1);
	set<string> temp2 = GetSet(rel2);

	for(auto it1 = temp1.begin(); it1 != temp1.end(); ++it1) {
		for(auto it2 = temp2.begin(); it2 != temp2.end(); ++it2) {
			relations[*it1].set.insert(*it2);
		}
	}

	for(auto it1 = temp2.begin(); it1 != temp2.end(); ++it1) {
		for(auto it2 = temp1.begin(); it2 != temp1.end(); ++it2) {
			relations[*it1].set.insert(*it2);
		}
	}
}
string Statistics::RelLookup(string att) const {
	try {
		return lookup.at(att);
	}
	catch(out_of_range &e) {
		return "";
	}
}

set<string> Statistics::GetSet(string rel) {
	return relations.at(rel).set;
}
bool Statistics::JoinValidate(struct AndList *parseTree, const char **relNames, int numToJoin) {
	bool atts = ConfirmAction(parseTree, relNames, numToJoin);
	bool sets = GetCombinedSets(relNames, numToJoin);

	return  atts && sets;
}

bool Statistics::ConfirmAction(struct AndList *parseTree, const char **relNames, int numToJoin) {
	struct AndList *andList = parseTree;
	struct OrList *orList = NULL;
	struct Operand *leftOp;
	struct Operand *rightOp;
	vector<string> left;
	vector<string> right;
	set<string> rels;

	for(int i = 0; i < numToJoin; ++i) {
		rels.insert(relNames[i]);
	}

	while(andList != NULL) {
		orList = andList->left;
		while(orList != NULL) {
			leftOp = orList->left->left;
			rightOp = orList->left->right;
			if( (leftOp->code == NAME && ParseOperand(leftOp->value, left)) == false || // left operand
					(rightOp->code == NAME && ParseOperand(rightOp->value, right) == false)) // right operand
				return false;

			if(leftOp->code == NAME && rels.count(left[0]) == 0) return false;
			if(rightOp->code == NAME && rels.count(right[0]) == 0) return false;

			orList = orList->rightOr;
		}
		andList = andList->rightAnd;
	}
	return true;
}

bool Statistics::GetCombinedSets(const char **relNames, int numToJoin) {
	set<string> superset;
	set<string> temp;
	for(int i = 0; i < numToJoin; ++i) {
		temp = GetSet(relNames[i]);
		for(auto it = temp.begin(); it != temp.end(); ++it) {
			superset.insert(*it);
		}
	}

	for(int i = 0; i < numToJoin; ++i) {
		superset.erase(relNames[i]);
	}
	return superset.empty();
}

bool Statistics::ParseOperand(string operand, vector<string> &out) const {
	string relation;
	string attribute;
	out.clear();

	if(operand.find(".") == string::npos) {
		attribute = operand;
		relation = RelLookup(attribute);
	}
	else {
		attribute = operand.substr(operand.find(".") + 1, operand.length() - operand.find("."));
		relation = operand.substr(0, operand.find("."));
	}
	if(GetUniqueVals(relation.c_str(), attribute.c_str()) == -1) {
		return false;
	}
	out.push_back(relation);
	out.push_back(attribute);

	return true;
}

void Statistics::MakeStatements(ComparisonOp op, std::vector<Statements*>& expressions,
	 	std::set<std::string>& relations) {
	int litCount = IsLiteral(op.left->code) ? 1 : 0;
	litCount += IsLiteral(op.right->code) ? 1 : 0;
	if(litCount == 2) {
		throw std::runtime_error("Statistics::MakeStatements: comparison with 2 literals");
	}
	int nameCount = IsName(op.left->code) ? 1 : 0;
	nameCount += IsName(op.right->code) ? 1 : 0;
	if(nameCount == 2) {
		vector<string> leftOp;
		if(!ParseOperand(op.left->value, leftOp)) {
			string str(op.left->value);
			throw std::runtime_error("Statistics::MakeStatements: could not parse name " + str);
		}
		vector<string> rightOp;
		if(!ParseOperand(op.right->value, rightOp)) {
			string str(op.right->value);
			throw std::runtime_error("Statistics::MakeStatements: could not parse name " + str);
		}
		BinaryStatements* be = new BinaryStatements(*this, leftOp[0], leftOp[1], rightOp[0], rightOp[1], op.code);
		expressions.push_back(be);
		relations.insert(leftOp[0]);
		relations.insert(rightOp[0]);
	} else {
		Operand* nameOp = IsName(op.left->code) ? op.left : op.right;
		Operand* litOp = IsLiteral(op.left->code) ? op.left : op.right;
		string litVal = *new string(litOp->value);
		vector<string> name;
		if(!ParseOperand(nameOp->value, name)) {
			string str(op.right->value);
			throw std::runtime_error("Statistics::MakeStatements: could not parse name " + str);
		}
		UnaryStatements* ue = new UnaryStatements(*this, name[0], name[1], litVal, op.code);
		expressions.push_back(ue);
		relations.insert(name[0]);
	}
}

bool Statistics::IsLiteral(int code) {
	return code == STRING || code == INT || code == STRING;
}

bool Statistics::IsName(int code) { return code == NAME; }

double Statistics::Join(OrList* orList, std::set<std::string>& relations) {
	OrList* curr = orList;
	vector<Statements*> expressions;

	while(curr != NULL && curr->left != NULL) {
		MakeStatements(*curr->left, expressions, relations);
		curr = curr->rightOr;
	}

	CombineStatementss(expressions);
	return ComputeNumTuples(expressions);
}

void Statistics::CombineStatementss(std::vector<Statements*>& expressions) {
	for(int i = 0; i < expressions.size(); ++i) {
		int j = 0;
		while(j < expressions.size()) {
			if(i < j && expressions[i]->Combine(*expressions[j])) {
				expressions.erase(expressions.begin()+j);
			} else {
				++j;
			}
		}
	}
}

double Statistics::ComputeNumTuples(std::vector<Statements*>& expressions) {
	if(expressions.size() == 1) {
		return expressions[0]->Tuples();
	}
	double sum = 0;
	double denominator = 1;

	for(int i = 0; i < expressions.size(); ++i) {
		sum += expressions[i]->Tuples();
		denominator *= expressions[i]->Denominator();
	}
	sum -= expressions[0]->Numerator() / denominator;

	return sum;
}

int Statistics::RelationSize() {
	return relations.size();
}
