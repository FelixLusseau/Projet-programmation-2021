#ifndef ARGS_PARSER_H
#define ARGS_PARSER_H

#include "program.h"

void printUsage(void);

int parseArgs(int argc, char ** argv, options_t * options);

#endif