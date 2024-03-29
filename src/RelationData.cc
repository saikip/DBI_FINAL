/*---
 * Title        : Project - 4,
 * Course       : DBI
 * Team Members : Priyam Saikia, Ruchika Mishra
*/
#include "RelationData.h"
#include <unordered_map>
#include <stdio.h>
#include <iostream>

using namespace std;

RelationMetaData::RelationMetaData() {}

RelationMetaData::~RelationMetaData() {}

RelationMetaData::RelationMetaData(const RelationMetaData &copyMe): dbLocation(copyMe.dbLocation),
		schemaLocation(copyMe.schemaLocation), schema(copyMe.schema) {}

RelationMetaData::RelationMetaData(const string &_dbLocation, const string &_schemaLocation,
		const Schema &_schema): dbLocation(_dbLocation), schemaLocation(_schemaLocation),
				schema(_schema) {}

RelationData::RelationData() {}

RelationData::RelationData(const RelationData &copyMe): data(copyMe.data) {}

RelationData::~RelationData() {
	// TODO Auto-generated destructor stub
}

void RelationData::Insert(const string &rel, const string &dbLocation, const string &schemaLocation,
		const Schema &schema) {
	data.insert({rel, RelationMetaData(dbLocation, schemaLocation, schema)});
}

void RelationData::Insert(const std::string &rel, const RelationMetaData &copy) {
	data.insert({rel, copy});
}

string RelationData::ToString() const {
	string out = "";
	for(auto it = data.begin(); it != data.end(); it++) {
		string rel = it->first;
		out.append(rel).append("\n");
	}
	return out;
}

void RelationData::Remove(const std::string &rel) {
	data.erase(rel);
}
