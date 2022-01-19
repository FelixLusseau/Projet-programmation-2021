#ifndef READ_BINARY_H
#define READ_BINARY_H

#include "program.h"

/**
 * @brief Read the number of entries in the binary at the beginning of the
 * binary file
 *
 * @param options
 * @return int
 */
int nbEntriesBin(options_t *options);

/**
 * @brief Reads an entry from the binary file ant put it on the pointer
 *
 * @param options
 * @param structureBase
 * @param precAuthornb
 */
void readStructure(options_t *options, structureBase_t *structureBase,
                   int16_t precAuthornb);

/**
 * @brief Reads the binary and print the structure if print==1 or return the
 * number of lines
 *
 * @param options
 * @param print
 * @return error_t
 */
error_t readEntireBin(options_t *options);

/**
 * @brief Prints the structureBase structure given in argument
 *
 * @param structureBase
 */
void printStruct(structureBase_t *structureBase);

#endif