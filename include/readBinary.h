#ifndef READ_BINARY_H
#define READ_BINARY_H

#include "program.h"

/**
 * @brief Read an entry from the binary base
 * 
 * @param options 
 * @param structureBase 
 * @param precAuthornb 
 */
void readStructure(options_t *options, structureBase_t *structureBase, int16_t precAuthornb);

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

# endif