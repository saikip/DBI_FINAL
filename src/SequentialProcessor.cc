/*---
 * Title        : Project - 2b,
 * Course       : DBI
 * Team Members : Priyam Saikia, Ruchika Mishra
*/

#include "SequentialProcessor.h"

SequentialProcessor::SequentialProcessor(): file(file), cursor(cursor), cursorIndex(cursorIndex), lastIndex(0) {}

SequentialProcessor::SequentialProcessor(File& _file, Page& _cursor, off_t& _cursorIndex, const off_t& _lastIndex) :
	file(_file), cursor(_cursor), cursorIndex(_cursorIndex), lastIndex(_lastIndex) {}

SequentialProcessor::SequentialProcessor(File& _file, Page& _cursor, off_t& _cursorIndex) :
	file(_file), cursor(_cursor), cursorIndex(_cursorIndex), lastIndex(NEGATIVE_ONE) {}


SequentialProcessor::~SequentialProcessor() {}

int SequentialProcessor::GetNext(Record& rec) {
	if(GetLength() == 0) {
		
		return 0;
	}

	if(cursor.GetFirst(&rec)) {
		
		return 1;
	} else {
		cursorIndex++;
		while(cursorIndex < GetLength()) {
			cursor.EmptyItOut();
			file.GetPage(&cursor, cursorIndex);
			if(cursor.GetFirst(&rec)) {
				return 1;
			}
			cursorIndex++;
		}
		cursorIndex--;
		return 0;
	}
}

int SequentialProcessor::GetLength() {
	off_t zero = 0;
	return lastIndex == -1 ? max(zero, file.GetLength()-1) : lastIndex+1;
}
