#ifndef READ_BINARY_H
#define READ_BINARY_H

#include "program.h"

/**
 * @brief Read an entry from the binary file ant put it on the pointer
 *
 * @param options
 * @param structureBase
 * @param precAuthornb
 */
void readStructure(options_t *options, structureBase_t *structureBase,
                   int16_t precAuthornb);

/**
 * @brief read the binary and print the structures if print==1
 *
 * @param options
 * @param print
 * @return int
 */
error_t readEntireBin(options_t *options, int print);

/**
 * @brief print the structureBase structure given in argument
 *
 * @param structureBase
 */
void printStruct(structureBase_t *structureBase);

#endif