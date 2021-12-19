#ifndef SEARCHING_FUNCTIONS_H
#define SEARCHING_FUNCTIONS_H

#include "program.h"

/**
 * @brief print the articles written by the author given in argument
 *
 * @param options
 * @return int
 */
int showArticles(options_t *options);

/**
 * @brief print the authors' names containing the string given in argument
 *
 * @param options
 * @return int
 */
int showAuthors(options_t *options, node **hashTable);

#endif