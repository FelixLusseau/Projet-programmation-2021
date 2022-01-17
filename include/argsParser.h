#ifndef ARGS_PARSER_H
#define ARGS_PARSER_H

#include "program.h"

/**
 * @brief print fonction usage
 *
 */
void printUsage(void);

/**
 * @brief parse the arguments from the command line
 *
 * @param argc
 * @param argv
 * @param options
 * @return error_t
 */
error_t parseArgs(int argc, char **argv, options_t *options);

#endif