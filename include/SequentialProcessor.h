#ifndef SequentialProcessor_H
#define SequentialProcessor_H 

#include "Record.h"
#include "File.h"

class SequentialProcessor {
	friend class MockSequentialProcessor;
private:
	File& file;
	Page& cursor;
	off_t& cursorIndex;
	const off_t& lastIndex;
	const off_t NEGATIVE_ONE = -1;
	int GetLength();
	SequentialProcessor();
public:
	/*
	 * Reads from the first record of cursor, and records from subsequent pages
	 * in file until the end of the file
	*/
	SequentialProcessor(File& file, Page& cursor, off_t& cursorIndex);
	/*
	 * Reads from the first record of cursor, and records from subsequent pages
	 * in file up to and including lastIndex.
	*/
	SequentialProcessor(File& file, Page& cursor, off_t& cursorIndex, const off_t& lastIndex);
	virtual ~SequentialProcessor();
	
	/*
	 * Gets the next record from the file. Returns 1 if a record was
	 * found 0 otherwise.
	*/
	virtual int GetNext(Record& rec);
};

#endif
