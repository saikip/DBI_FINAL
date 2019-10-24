#ifndef COMPARISON_H
#define COMPARISON_H

#include "Record.h"
#include "Schema.h"
#include "File.h"
#include "Comparison.h"
#include "ComparisonEngine.h"
#include "SQL.h"
#include <string>
#include <vector>


// This stores an individual comparison that is part of a CNF
class Comparison {

	friend class ComparisonEngine;
	friend class CNF;
	
	Target operand1;
	int whichAtt1;
	Target operand2;
	int whichAtt2;

	Type attType;

	CompOperator op;

public:

	Comparison();

	//copy constructor
	Comparison(const Comparison &copyMe);

	// print to the screen
	void Print ();

	std::string ToString(); 
};


class Schema;

// This structure encapsulates a sort order for records
class OrderMaker {

	friend class ComparisonEngine;
	friend class CNF;
	
	int numAtts;

	int whichAtts[MAX_ANDS];
	Type whichTypes[MAX_ANDS];

public:
	
	// creates an empty OrdermMaker
	OrderMaker();

	// create an OrderMaker that can be used to sort records
	// based upon ALL of their attributes
	OrderMaker(Schema *schema);

	OrderMaker(const Schema *base, const Schema *order);

	// Similar to the above
	OrderMaker(const Schema *base, const std::vector<std::string> *order);

	// create an OrderMaker that can be used to sort records
	// based upon str which is expected to have the following format:
	// OrderMaker = (whichAtt whichType)*
	OrderMaker(std::string str);

	virtual ~OrderMaker();

	virtual int GetNumAtts();
	virtual int GetAttIndex(int i);
	virtual Type GetAttType(int i);
	virtual void Print ();
	virtual std::string ToString();
};

class Record;

// This structure stores a CNF expression that is to be evaluated
// during query execution

class CNF {

	friend class ComparisonEngine;

	Comparison orList[MAX_ANDS][MAX_ORS];
	
	int orLens[MAX_ANDS];
	int numAnds;

public:
	virtual ~CNF();
	
	virtual int GetSortOrders (OrderMaker &left, OrderMaker &right);

	virtual int GetSortOrder (OrderMaker &order);


	virtual bool MakeQuery(const OrderMaker &sortOrder, OrderMaker &query);

	virtual bool IsSortableAttribute(const int &attr);
	
	
	virtual void Print ();
	std::string ToString (const std::string& prefix); 

        virtual void GrowFromParseTree (const struct AndList *parseTree, const Schema *leftSchema,
		const Schema *rightSchema, Record &literal);

        virtual void GrowFromParseTree (const struct AndList *parseTree, const Schema *mySchema,
		Record &literal);

};

#endif
