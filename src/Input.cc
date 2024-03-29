#include <sstream>
#include "Input.h"

Input::Input() {
}

Input::~Input() {
}

bool Input::Open(std::string fname) {
	filename = fname;
	std::string mode("r+");
	if(!FileExists(fname.c_str())) {
		mode = "w+";
	}
	file = fopen(fname.c_str(), mode.c_str());
	return file != NULL;
}

bool Input::Open(FILE *_file) {
	file = _file;
	return true;
}

bool Input::Close() {
	int ret = fclose(file);
	file = NULL;
	return ret == 0;
}

int Input::Read(void* buf, size_t count) {
	return fread(buf, 1, count, file);
}

int Input::Write(const void* buf, size_t count) {
	return fwrite(buf, 1, count, file);
}

bool Input::ReadLine(std::string* line) {
	std::stringstream ss;
	bool newLineFound = false;
	// append chars until a newline is found
	while(!newLineFound) {
		int bytesRead = fread(buffer, 1, sizeof(buffer), file);
		if(bytesRead == 0) {
			// we've reached the end of the file
			break;
			// return false;
		}
		for(int i = 0; i < bytesRead; ++i) {
			if(buffer[i] == '\n' || buffer[i] == '\r') {
				// move the file cursor to the byte past this newline
				fseek(file, i-bytesRead+1, SEEK_CUR);
				newLineFound = true;
				break;
			} else {
				// not a newline, add the character to the output
				ss << buffer[i];
			}
		}
	}
	line->clear();
	for(int i = 0; i < ss.str().size(); ++i) {
		line->push_back(ss.str()[i]);
	}
	return ss.str().size() > 0;
}

bool Input::Append(std::string value) {
	long pos = ftell(file);
	fseek(file, 0, SEEK_END);
	int bytesWritten = fwrite(value.c_str(), 1, value.size(), file);
	fseek(file, pos, SEEK_CUR);
	return bytesWritten == value.size();
}

bool Input::Truncate() {
	int ret = fclose(file);
	if(ret != 0) return false;
	file = fopen(filename.c_str(), "w+");
	return file != NULL;
}

void Input::LSeek(off_t offset) {
	fseek(file, offset, SEEK_SET);
}

bool Input::FileExists(const char* fileName) {
	return access(fileName, F_OK) != -1;
}

void Input::MakeTemp(char *scheme) {
	int fd = mkstemp(scheme);
	close(fd);
}

int Input::Rename(const char *oldName, const char *newName) {
	return rename(oldName, newName);
}

int Input::Remove(const char *name) {
	return remove(name);
}
