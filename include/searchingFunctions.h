#ifndef SEARCHING_FUNCTIONS_H
#define SEARCHING_FUNCTIONS_H

#include "program.h"

/**
 * @brief print the articles written by the author given in argument
 *
 * @param options
 * @return int
 */
int showArticles(options_t *options, node **hashTable, node *node0);

/**
 * @brief print the authors' names containing the string given in argument
 *
 * @param options
 * @return int
 */
int showAuthors(options_t *options, node **hashTable);

/**
 * @brief print the authors' names containing the string given in argument using
 * the graph
 *
 * @param options
 * @param hashTable
 * @param node0
 * @return int
 */
int showAuthorsGraph(options_t *options, node **hashTable, node *node0);

#endif