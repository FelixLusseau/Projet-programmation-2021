#ifndef READ_BINARY_H
#define READ_BINARY_H

#include "program.h"

/**
 * @brief read the XML to count lines
 *
 * @param options
 * @return int
 */
int readEntireXML(options_t *options);

/**
 * @brief Read an entry from the binary base
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
int readEntireBin(options_t *options, int print);

/**
 * @brief print the structure given in argument
 *
 * @param structureBase
 */
void printStruct(structureBase_t *structureBase);

#endif