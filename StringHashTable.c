// On my honor:
//
// - I have not discussed the C language code in my program with
// anyone other than my instructor or the teaching assistants 
// assigned to this course.
//
// - I have not used C language code obtained from another student, 
// the Internet, or any other unauthorized source, either modified
// or unmodified. 
//
// - If any C language code or documentation used in my program 
// was obtained from an authorized source, such as a text book or
// course notes, that has been clearly noted with a proper citation
// in the comments of my program.
//
// - I have not designed this program in such a way as to defeat or
// interfere with the normal operation of the Curator System.
//
// <Abrar Islam>
// <abrarr18>
// hashTable.c
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "StringHashTable.h"

#define defaultNumOffsets 4



// Node type for the chained hash table for C-strings.
struct _StringNode {
	char*     key;              // ptr to proper C-string
	uint32_t* locations;        // ptr to dynamic array of string locations
	uint32_t  numLocations;     // number of elements currently in locations array
	uint32_t  maxLocations;     // dimension of locations array
	struct _StringNode* next;   // ptr to next node object in table slot
};

// Declare "private" helper functions here (one is already included):
static void StringNode_display(FILE* fp, const StringNode* const pNode);

StringNode* createStringNode (char* key, uint32_t location);
void addNode(StringNode* origin, StringNode* new);
StringNode* getNode(StringNode* origin, uint32_t index);

///////////////////////////////////////////////////////////////// StringNode implementation
//
// You should write implementations of any "private" functions related to the
// StringNode type here; FWIW the reference solution includes two such functions
// in addition to the StringNode_display() function that's given:
//

/** Writes a formatted display of the fields of a StringNode object; for
 *  example:
 *            New Tank:    [359569, 411752, 857960, 942772]
 * 
 *  Pre:  fp is open on an output file
 *        pNode points to a valid StringNode object
 */
static void StringNode_display(FILE* fp, const StringNode* const pNode) {
	
	assert( pNode != NULL );
	
	fprintf(fp, "     %s:  ", pNode->key);
	
   fprintf(fp, "  [%"PRIu32, pNode->locations[0]);
   for (uint32_t idx = 1; idx < pNode->numLocations; idx++) {
	   fprintf(fp, ", %"PRIu32, pNode->locations[idx]);
   }
   fprintf(fp, "]\n");
}


///////////////////////////////////////////////////////////////// StringHashTable implementation
//
// You should complete the implementations of the "public" functions related to the
// StringHashTable type here:
//

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
StringHashTable* StringHashTable_create(uint32_t Size, 
                                        uint32_t (*Hasher)(const char* str)) {
	
	// Modify the following code as needed:
	
	StringHashTable* newTable = calloc(1, sizeof(StringHashTable));
	newTable->tableSz = Size; 
	newTable->numEntries = 0; 
	newTable->hash = Hasher; 
	//Initialize table
	newTable->table = calloc(newTable->tableSz, sizeof(StringNode*) * Size);
	
	return newTable;
}

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
 **/

bool StringHashTable_addEntry(StringHashTable* const pTable, 
                              char* key, uint32_t location) 
{
  	int hashIndex = pTable->hash(key)% pTable->tableSz;
  	StringNode* ptr = pTable->table[hashIndex];
  	while( ptr != NULL)
  	{
		if(strcmp(ptr->key, key) == 0){
			for (int i=0; i< ptr->numLocations; i++){
				if (location == ptr->locations[i]){
					return false; 
				}
				
			}
			if (ptr->numLocations == ptr->maxLocations){
				ptr->maxLocations *= 2; 
				ptr->locations = realloc(ptr->locations, ptr->maxLocations * sizeof(uint32_t));  
			}
			ptr->locations[ptr->numLocations] = location; 
		ptr->numLocations++; 
		return true; 
		}
		ptr = ptr->next; 
	} 
	StringNode* newNode = createStringNode(key, location);
	newNode->next = pTable->table[hashIndex];
	pTable->table[hashIndex] = newNode; 
	pTable->numEntries++; 
	return true; 
}
 
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
uint32_t* StringHashTable_getLocationsOf(const StringHashTable* const pTable, 
                                         const char* key) {
											 
	int hashIndex = pTable->hash(key)% pTable->tableSz;
  	StringNode* ptr = pTable->table[hashIndex];
  	while( ptr != NULL)
  	{
		if(strcmp(ptr->key, key) == 0){
			uint32_t* copy = calloc(ptr->numLocations+1, sizeof(uint32_t));
			memcpy(copy, ptr->locations, ptr->numLocations* sizeof(uint32_t)); 
			return copy; 
		}
		ptr = ptr->next; 
	}
	return NULL; 
}

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
void StringHashTable_clear(StringHashTable* const pTable) {
	uint32_t i =0; 
	while(i < pTable->tableSz)
	{
		StringNode* curr = pTable->table[i]; 
		while(curr != NULL)
		{
			StringNode* temp = curr; 
			free(curr->key); 
			free(curr->locations); 
			curr = curr->next; 
			free(temp); 
		}
		i++; 
	}
	
	free(pTable->table); 
     /**
	for(uint32_t i =0; i < pTable->tableSz ;i++){
		
		free(pTable->table[i]);
	}
**/
}

/** Writes a clearly-formatted display of the contents of a given
 *  StringHashTable.
 * 
 *  Pre:  fp points to an open file, or is stdout
 *        pTable points to a proper StringHashTable object
 */
void StringHashTable_display(FILE* fp, const StringHashTable* const pTable) {
	
	fprintf(fp, "Hash table contains the following %"PRIu32" entries:\n", 
	            pTable->numEntries);
	
	uint32_t pos = 0;
	while ( pos < pTable->tableSz ) {
		
		StringNode* currEntry = pTable->table[pos];
		if ( currEntry != NULL ) {
		
   		fprintf(fp, "%5"PRIu32": ", pos);
   		StringNode_display(fp, currEntry);
	      currEntry = currEntry->next;
   		while ( currEntry != NULL ) {
				fprintf(fp, "       ");
      		StringNode_display(fp, currEntry);
		      currEntry = currEntry->next;
			}
			fprintf(fp, "\n");
		}
		pos++;
	}
}

//
// You should write implementations of any "private" functions related to the
// StringHashTable type here; FWIW the reference solution includes one such 
// function.
//
StringNode* createStringNode (char* key, uint32_t location)
{
	StringNode* new = malloc(sizeof(StringNode)); 
	new->key = calloc(strlen(key)+1, sizeof(char)); 
	strcpy(new->key, key);  
	new->locations = calloc(4,sizeof(uint32_t));
    new->locations[0] = location;
	new->numLocations = 1 ; 
	new-> maxLocations= 4 ;
	
	return new;  
	
}

StringNode* getNode(StringNode* origin, uint32_t index)
{
   StringNode* ptr = origin;
   uint32_t i = 0;
   while(ptr->next != NULL && i < index)
   {
       ptr = ptr->next;
       i++;
   }
   return ptr;
}

void addNode(StringNode* origin, StringNode* new)
{
    StringNode* ptr = origin; 
	while(ptr->next != NULL)
	{
		ptr = ptr->next; 
        ptr->numLocations++;
        if(ptr->numLocations > ptr->maxLocations)
        {
          free(new);
          new = NULL;
          break;
        }
	}
	ptr->next = new; 
} 

void clearList(StringNode* head) 
{
	StringNode* ptr = head; 
	StringNode* trail = head; 
	
	while(ptr->next != NULL)
	{
		ptr = ptr->next; 
		free(trail); 
		trail = ptr; 
	}
	free(ptr); 
	ptr->numLocations = 0; 	
}

