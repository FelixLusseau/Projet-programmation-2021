#ifndef BASE_PARSER_H
#define BASE_PARSER_H

#include "program.h"

/**
 * @brief function to parse the xml base into the structureBase_t structure written in the binary base
 * 
 * @param options 
 * @return int 
 */
int parseBase(options_t * options);

/**
 * @brief initialise the structure with the NULL values corresponding to the types
 * 
 * @param structureBase 
 * @param authornb 
 */
void initStructure(structureBase_t * structureBase, int authornb);

/**
 * @brief extract the author from a line containing the <author> tag
 * 
 * @param structureBase 
 * @param line 
 */
void extractAuthor(structureBase_t * structureBase, char * line);

/**
 * @brief extract the year from a line containing the <year> tag
 * 
 * @param structureBase 
 * @param line 
 */
void extractYear(structureBase_t * structureBase, char * line);

/**
 * @brief extract the title from a line containing the <title> tag
 * 
 * @param structureBase 
 * @param line 
 * @param titleEndOfLine 
 */
void extractTitle1(structureBase_t * structureBase, char * line, int * titleEndOfLine);

/**
 * @brief extract the title from a line not beginning with a tag if the </title> wasn't find
 * 
 * @param structureBase 
 * @param line 
 * @param titleEndOfLine 
 */
void extractTitle2(structureBase_t * structureBase, char * line, int titleEndOfLine);

/**
 * @brief read the binary and print the structures
 * 
 * @param options 
 * @return int 
 */
int readEntireBin(options_t * options);

/**
 * @brief read one entry and return it in a structure
 * 
 * @param options 
 * @param curseur 
 * @return structureBase_t 
 */
structureBase_t readEntryBin(options_t * options, int curseur);

/**
 * @brief print the structure given in argument
 * 
 * @param structureBase 
 */
void printStruct(structureBase_t * structureBase);

/**
 * @brief print the articles written by the author given in argument
 * 
 * @param options 
 * @return int 
 */
int showArticles(options_t * options);

/**
 * @brief print the authors' names containing the string given in argument
 * 
 * @param options 
 * @return int 
 */
int showAuthors(options_t * options);

#endif