/*---
 * Title        : Project - 4,
 * Course       : DBI
 * Team Members : Priyam Saikia, Ruchika Mishra
*/
#include "GenericDBFile.h"

#ifndef INCLUDE_TREEDBFILE_H_
#define INCLUDE_TREEDBFILE_H_

/**
 *	TreeDBFile will store Records in a tree
 */
class TreeDBFile: public GenericDBFile {
friend class TreeDBFileTest;
private:

	/**
	 * This is a function will be called by the public Load and it will provide it's own Record.
	 * The Load logic will be put in here. The existence of this function is purely for testing
	 * purposes (to Mock record).
	 */
	virtual void Load (Schema &myschema, const char *loadpath, Record &record);

public:
	TreeDBFile(File &file, Input &rfile, Settings &config, ComparisonEngine &comp);
    virtual ~TreeDBFile();

    /**
	 * Appends the data located at loadpath to memory.
	 * @param myschema	The schema of the file being loaded to memory
	 * @param loadpath	The path to the file being loaded
	 */
	virtual void Load (Schema &myschema, const char *loadpath);

	/**
	 * Moves the pointer from the current record to the first record of the first page.
	 */
	virtual void MoveFirst ();

	/**
	 * Adds addMe to the last page in memory. Writes are not immediately readable. In order to get
	 * the most immediate written records, you must call MoveFirst.
	 * @param addme	The Record being added to DBFile
	 */
	virtual void Add (Record &addme);

	/**
	 * Puts the current record in fetchme and moves the pointer to the next record. Upon use, the
	 * record referenced by fetchme is consumed and the new record in fetchme is removed from DB
	 * File.
	 * @param fetchme	A reference that will be used to return the current Record
	 * @return			0 if there is no valid record to be returned.
	 */
	virtual int GetNext (Record &fetchme);

	/**
	 * Looks for the next record that passes applyMe when applied between a Record in DBFile and
	 * literal. This function increments the pointer accordingly.
	 * @param fetchme	A reference that will be used to return the current Record
	 * @param applyMe	The CNF that will be applied between the Record(s) in DBFile and literal
	 * @param literal	The Record that will be compared with the Records in DBFile.
	 * @return			0 if there is no valid record to be returned.
	 */
	virtual int GetNext (Record &fetchme, CNF &cnf, Record &literal);

	/**
	 * Writes out any changes to disk
	 */
	virtual void Flush();

	/**
	 * Initializes cursor and last.
	 */
	virtual void Initialize();

	/**
	 * Puts DBFile into an initial state.
	 */
	virtual void Reset();
};

#endif /* INCLUDE_SORTEDDBFILE_H_ */
