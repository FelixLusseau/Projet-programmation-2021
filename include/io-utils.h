#ifndef IO_UTILS_H
#define IO_UTILS_H

#include "fonctionsMatrice.h"
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

int endOfProgram(options_t *options, node *node0, node **hashTable);

/**
 * @brief convert error from error_t to string
 *
 * @param err
 * @return const char*
 */
const char *errorToString(error_t err);

void progressBar(int n);

#endif