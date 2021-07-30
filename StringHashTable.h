#ifndef STRINGHASHTABLE_H
#define STRINGHASHTABLE_H
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <stdbool.h>

/***  DO NOT MODIFY THIS FILE IN ANY WAY!!  ***/

// The following two lines provide a forward declaration of StringNode,
// allowing the use of StringNode as a type name in the declaration of
// the StringHashTable type that follows, but without revealing anything
// about the internals of a StringNode object.  The full definition of
// the StringNode type is contained in StringHashTable.c.
struct _StringNode;
typedef struct _StringNode StringNode;

/** StringHashTable provides a user-configurable hash table implementation.
 * 
 *  The user can specify the size of the hash table, but the size is then
 *  fixed, so the user must be careful to make sure the table is large
 *  enough.  
 * 
 *  As the name implies, the key field must be a zero-terminated C-string.
 * 
 *  The user must also provide the hash function to be applied to the
 *  record keys.
 */
struct _StringHashTable {
	
	StringNode** table;       // physical array for table
	uint32_t tableSz;         // number of slots in the table
	uint32_t numEntries;      // number of entries in the table (not
	                          //   necessarily the number of nonempty slots)
	
	uint32_t (*hash)(const char* str);   // pointer to the hash function
};
uint32_t elfhash(const char* str);//added later
typedef struct _StringHashTable StringHashTable;

/** Creates a new StringHashTable object such that:
 *    - the array has dimension Size
 *    - the slots in the array are set to NULL
 *    - the hash pointer is set to Hasher (so the hash table will use
 *      that user-supplied hash function)
 * 
 *  Pre: Size equals the desired number of slots in the table
 *       Hasher is the name of the hash function the table is to use,
 *         and that function conforms to the specified interface
 * 
 *  Returns: pointer to a newly-allocated StringHashTable object;
 *           blows up an assert() if creation fails
 */
StringHashTable* StringHashTable_create(uint32_t Size, uint32_t (*Hasher)(const char* str));

/** Adds an entry to the hash table corresponding to the given key/location.
 *  
 *  Pre:  pTable points to a proper StringHashTable object
 *        key points to a proper C-string
 *        location is set to a corresponding location
 * 
 *  Post: if the key/location pair is already in the table, fails; otherwise
 *        if the table does not contain an entry for key, a new index entry
 *           has been created and location has been installed in it;
 *        otherwise, location has been installed into the existing index entry
 *           for key;
 *        the user's C-string is duplicated, not linked to the table
 * 
 *  Returns: true iff the key/location have been properly added to the table
 */
bool StringHashTable_addEntry(StringHashTable* const pTable, char* key, uint32_t location);

/** Finds the locations, if any, that correspond to the given key.
 * 
 *  Pre:  pTable points to a proper StringHashTable object
 *        key points to a proper C-string
 * 
 *  Returns: NULL if there is no entry for key in the table; otherwise
 *           a pointer to an array storing the locations corresponding
 *           to the key, and terminated by a zero location (which would
 *           never be logically valid)
 */
uint32_t* StringHashTable_getLocationsOf(const StringHashTable* const pTable, const char* key);

/** Deallocates memory associated with a StringHashTable.
 * 
 *  Pre:  pTable points to a proper StringHashTable object
 * 
 *  Post: the following have been deallocated
 *           - the key belonging to every index entry in the table
 *           - the array of locations belonging to every index entry in the table
 *           - every index entry in the table
 *           - the array for the table
 *
 *   Note: the function does not attempt to deallocate the StringHashTable object
 *         itself, because that object may or may not have been allocated dynamically;
 *         cleanup that up is the responsibility of the user.
 */
void StringHashTable_clear(StringHashTable* const pTable);

/** Writes a clearly-formatted display of the contents of a given
 *  StringHashTable.
 * 
 *  Pre:  fp points to an open file, or is stdout
 *        pTable points to a proper StringHashTable object
 */
void StringHashTable_display(FILE* fp, const StringHashTable* const pTable);

#endif
