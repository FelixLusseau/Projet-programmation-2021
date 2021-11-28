#ifndef BASE_PARSER_H
#define BASE_PARSER_H

#include "program.h"

void handleSignal();

void initSigaction();

int parseBase(options_t * options);

void initStructure(structureBase_t * structureBase, int authornb);

void extractAuthor(structureBase_t * structureBase, int * authornb, char * line);

void extractYear(structureBase_t * structureBase, char * line);

void extractTitle1(structureBase_t * structureBase, char * line, int * titleLength, int * titleEndOfLine);

void extractTitle2(structureBase_t * structureBase, char * line, int * titleLength, int titleEndOfLine);

int readEntireBin(options_t * options);

structureBase_t readEntryBin(options_t * options, int curseur);

#endif