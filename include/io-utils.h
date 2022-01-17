#ifndef IO_UTILS_H
#define IO_UTILS_H

#include "analyseGraph.h"
#include "program.h"

/**
 * @brief implement action when CTRL+C is pressed
 *
 */
void handleSignal();

/**
 * @brief implement CTRL+C personnalised interruption
 *
 */
void initSigaction();

/**
 * @brief Initialise the Options structure
 *
 * @param options
 */
void initOptions(options_t *options);

/**
 * @brief Verify that the input file is a xml base
 *
 * @param file
 * @return int
 */
int isXML(FILE *file);

/**
 * @brief Open the files given through the arguments
 *
 * @param options
 * @param openMode
 * @param test
 * @return int
 */

int openFiles(options_t *options, char *openMode, int test);

/**
 * @brief Close the files given through the arguments
 *
 * @param options
 * @return int
 */

int closeFiles(options_t *options);

/**
 * @brief Close the files and free the table and graph at the end of the program
 *
 * @param options
 * @param node0
 * @param hashTable
 * @return int
 */
void endOfProgram(options_t *options, node *node0, node **hashTable);

/**
 * @brief convert error from error_t to string
 *
 * @param err
 * @return const char*
 */
const char *errorToString(error_t err);

/**
 * @brief Print a magenta progress bar of 100 symbols
 *
 * @param n
 */
void progressBar(int n);

#endif