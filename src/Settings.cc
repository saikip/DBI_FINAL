#include "../include/Settings.h"
#include "../include/Input.h"
#include <iostream>

using namespace std;

Settings::Settings() {}

Settings::~Settings() {}

bool Settings::Read(Input &file) {
    bool success = true;
    std::string* line = new std::string();
    std::string key;
    std::string val;
    int delim;

    file.LSeek(0); 
    while(success && file.ReadLine(line)) {
        delim = line->find_first_of("=");
        if(delim == string::npos) success = false;
        else {
            key = line->substr(0, delim);
            val = line->substr(delim + 1, line->length() + 1);
            map.insert(pair<string, string>(key, val));
        }
    }
    delete line;
    return success;
}

bool Settings::Write(Input &file) {
    bool success = file.Truncate();
    if(success) file.LSeek(0); 

    for(std::map<string, string>::iterator it = map.begin(); success && it != map.end(); it++) {
        success = file.Append(it->first + "=" + it->second + "\n");
    }
    return success;
}

void Settings::AddKey(string key, string value) {
    if(key.find("=") != string::npos ||
            key.find("\r") != string::npos ||
            key.find("\n") != string::npos) {
        throw IllegalKeyException();
    }

    if(value.find("\r") != string::npos || value.find("\n") != string::npos) {
        throw IllegalValueException();
    }
    map.insert(pair<string, string>(key, value));
}

std::string Settings::GetKey(string key) {
    string value;
    try {
        value = map.at(key);
    } catch(out_of_range e) {
        value = "";
    }
    return value;
}

void Settings::ReplaceKey(string key, string value) {
	if(key.find("=") != string::npos ||
			key.find("\r") != string::npos ||
			key.find("\n") != string::npos) {
		throw IllegalKeyException();
	}

	if(value.find("\r") != string::npos || value.find("\n") != string::npos) {
		throw IllegalValueException();
	}
	map.erase(key);
	map.insert(pair<string, string>(key, value));
}

void Settings::Clear() {
	map.clear();
}
