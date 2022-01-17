#ifndef SEARCHING_FUNCTIONS_H
#define SEARCHING_FUNCTIONS_H

#include "program.h"

/**
 * @brief print the authors' names containing the string given in argument of
 * command using the graph (author0or1 allows to read the first or second
 * author's argument)
 *
 * @param options
 * @param node0
 * @param author0or1
 * @return int
 */
int showAuthors(options_t *options, node *node0, int author0or1);

/**
 * @brief Calls showAuthors and ask for a unique author name (author0or1 allows
 * to read the first or second author's argument). If it is not unique it enters
 * in a loop and if the author is invalid it prints an error
 *
 * @param options
 * @param node0
 * @param author0or1
 * @return error_t
 */
error_t chooseAuthor(options_t *options, node *node0, int author0or1);

/**
 * @brief Prints the articles written by the author given in argument and if
 * year!=0 it prints those of the year given
 *
 * @param options
 * @param node0
 * @param year
 * @return error_t
 */
error_t showArticles(options_t *options, node *node0, int year);

#endif