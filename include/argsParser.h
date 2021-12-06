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
 * @return int 
 */
int parseArgs(int argc, char ** argv, options_t * options);

#endif