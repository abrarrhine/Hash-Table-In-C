// Test/grading driver for c07 (string hash table) in CS 2505 Fall 2019
//
// Invocation options:  c07driver <name of GIS record file>
//                      c07driver <name of GIS record file> -repeat
// See the project specification for details.
//
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <inttypes.h>
#include <assert.h>

#include "StringHashTable.h"
#include "testHashTable.h"

// The following global variables (external linkage) are set by the
// grading code in the testHashTable module:
uint32_t buildChecksDone    = 0;
uint32_t buildChecksPassed  = 0;
uint32_t searchChecksDone   = 0;
uint32_t searchChecksPassed = 0;

// The following constants determine how the two phases of testing
// (table building and table searching) are weighted in computing
// the overall score:
const double buildWeight  = 0.50;
const double searchWeight = 0.50;

uint32_t elfhash(const char* str);

int main(int argc, char** argv) {
	
	if ( argc < 2 ) {
		printf("Invocation: driver <GIS record file> [-repeat]\n");
		return 1;
	}
	char* gisFileName = argv[1];
	
	uint32_t seed;
	if ( argc == 3 && strcmp(argv[2], "-repeat") == 0 ) {
		FILE* fp = fopen("seed.txt", "r");
		if ( fp == NULL ) {
			printf("Could not open seed.txt\n");
			printf("Read the instructions\n");
			return 2;
		}
		fscanf(fp, "%"PRIu32, &seed);
		fclose(fp);
	}
	else {
		seed = (uint32_t) time(NULL);
		FILE* fp = fopen("seed.txt", "w");
		fprintf(fp, "%"PRIu32"\n", seed);
		fclose(fp);
	}
	srand(seed);
	
	StringHashTable* stuTable = NULL;
	StringHashTable* refTable = NULL;
	if ( !testBuildingTable(gisFileName, &stuTable, &refTable, elfhash) ) {
		//free(newTable);
		return 0;
	}
	FILE* fp = fopen("table.txt", "w");
   StringHashTable_display(fp, refTable);
   fclose(fp);

	testSearchingTable(stuTable, refTable);
   StringHashTable_clear(stuTable);
   free( stuTable );
   
   // Log score information:
   FILE* fpScores = fopen("scores.txt", "w");
   fprintf(fpScores, "GIS file used: %s\n", argv[1]);
   fprintf(fpScores, "Table building: %"PRIu32" / %"PRIu32"\n", 
                                        buildChecksDone, buildChecksPassed);
   fprintf(fpScores, "Search testing: %"PRIu32" / %"PRIu32"\n", searchChecksDone,
                                        searchChecksPassed);
   double buildScore = 100.0*((double) buildChecksPassed / (double) buildChecksDone);
   double searchScore = 100.0*((double) searchChecksPassed / (double) searchChecksDone);
   double weightedScore = buildWeight*buildScore + searchWeight*searchScore;
   fprintf(fpScores, ">>Scores from testing<<\n");
   fprintf(fpScores, "1 >> Score for building:  %5.2f\n", buildScore);
   fprintf(fpScores, "2 >> Score for searching: %5.2f\n", searchScore);
   fprintf(fpScores, "3 >> Weighted score:      %5.2f\n", weightedScore);
   fclose(fpScores);
	
   return 0;
}

/** Hashes a C-string to an unsigned integer.
 *  Pre:
 *       str points to a zero-terminated char array
 */
uint32_t elfhash(const char* str) {

   assert(str != NULL );      // self-destuct if called with NULL
   
   uint32_t hashvalue = 0,    // value to be returned
            high;             // high nybble of current hashvalue
            
   while ( *str ) {           // continue until *str is '\0'
		
      hashvalue = (hashvalue << 4) + *str++;      // shift high nybble out,
                                                  //   and add in next char
      
      if ( (high = (hashvalue & 0xF0000000)) ) {  // if high nybble != 0000
         hashvalue = hashvalue ^ (high >> 24);    //   fold it back in
      }
      
      hashvalue = hashvalue & 0x7FFFFFFF;         // zero high nybble
   }
   
   return hashvalue;
}
