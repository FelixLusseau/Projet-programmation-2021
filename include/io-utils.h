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
 * @brief initialise CTRL+C personnalised interruption
 *
 */
void initSigaction();

/**
 * @brief Initialise the options structure with the null values corresponding to the
 * types
 *
 * @param options
 */
void initOptions(options_t *options);

/**
 * @brief Verify that the input file is a xml base searching the "<?xml" at the
 * beginning of the file
 *
 * @param file
 * @return error_t
 */
error_t isXML(FILE *file);

/**
 * @brief Verify that the output file is a binary or an empty file searching the
 * "<binary file>\n" at the beginning
 *
 * @param file
 * @return error_t
 */
error_t isBinOrEmpty(FILE *file);

/**
 * @brief Open the files given through the arguments
 *
 * @param options
 * @param openMode
 * @return error_t
 */

error_t openFiles(options_t *options, char *openMode);

/**
 * @brief Close the files given through the arguments if opened
 *
 * @param options
 * @return error_t
 */

error_t closeFiles(options_t *options);

/**
 * @brief Close the files, free the hashtable and the graph and print a message
 * if CTRL+C have been pressed at the end of the program
 *
 * @param options
 * @param node0
 * @param hashTable
 * @return int
 */
void endOfProgram(options_t *options, node *node0, node **hashTable);

/**
 * @brief convert error from error_t to string message
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