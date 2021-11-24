#ifndef BASE_PARSER_H
#define BASE_PARSER_H

#include "program.h"

int parseBase(options_t * options);

int readBin(options_t * options);

void initStructure(structureBase_t * structureBase, int authornb);

void extractAuthor(structureBase_t * structureBase, int authornb, char * line);

void extractYear(structureBase_t * structureBase, char * line);

#endif