#ifndef SETTINGS_H
#define SETTINGS_H

#include <iosfwd>
#include <map>
#include "Input.h"
#include <stdexcept>
/*---
 * Title        : Project - 1,
 * Course       : DBI
 * Team Members : Priyam Saikia, Ruchika Mishra
*/
class Settings {
friend class SettingsTest;
private:
    std::map<std::string, std::string> map;

public:
    Settings();
    virtual ~Settings();
    virtual bool Read(Input &file);
    virtual bool Write(Input &file);
    virtual void AddKey(std::string key, std::string value);
    virtual std::string GetKey(std::string key);
    virtual void ReplaceKey(std::string key, std::string value);
    virtual void Clear();
};

class IllegalKeyException: public std::runtime_error {
public:
    IllegalKeyException(std::string m = "ERROR") : std::runtime_error(m) { }
};

class IllegalValueException: public std::runtime_error {
public:
    IllegalValueException(std::string m = "ERROR") : std::runtime_error(m) { }
};

#endif
