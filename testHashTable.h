#ifndef TESTHASHTABLE_H
#define TESTHASHTABLE_H
#include <stdbool.h>
#include "StringHashTable.h"

bool testBuildingTable(const char* gisFileName, StringHashTable** stuTable,
                       StringHashTable** refTable, uint32_t (*Hasher)(const char* str));

/**
 *   This MUST be called AFTER testBuildingTable() has been called and reported
 *   that a correct hash table has been constructed using the student's hash
 *   table implementation.
 */
bool testSearchingTable(const StringHashTable* const stuTable,
                        StringHashTable* const refTable);

#endif
