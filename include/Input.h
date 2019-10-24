#ifndef INPUT_H
#define INPUT_H

#include <unistd.h>
#include <stdio.h>
#include <string>

class Input {
private:
	FILE* file = NULL;
	char buffer[4096];
	std::string filename;
public:
    Input();
    virtual ~Input();

    virtual bool Open(std::string fname);
    virtual bool Open(FILE *_file);
    virtual bool Close();
    virtual int Read(void* buf, size_t count);
    virtual int Write(const void* buf, size_t count);
    virtual bool ReadLine(std::string* line);
    virtual bool Append(std::string value);
    virtual bool Truncate();
    virtual void LSeek(off_t offset);
    virtual bool FileExists(const char* fileName);
    virtual int Rename(const char *oldName, const char *newName);
    virtual int Remove(const char *name);
    virtual void MakeTemp(char *scheme);
};

#endif INPUT_H
