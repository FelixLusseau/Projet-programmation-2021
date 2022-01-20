#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/signal.h>

#include "baseParser.h"
#include "program.h"
#include "readBin.h"

extern int interruptFlag;

int nbEntriesBin(options_t *options) {
    int nbEntries;
    fseek(options->outputFile, 14, SEEK_SET);
    int ret = fread(&nbEntries, sizeof(int), 1, options->outputFile);
    if (ret == -1) {
        nbEntries = 1;
        fprintf(stderr, "Unable to read nbEntries ! \n");
    }
    fseek(options->outputFile, 28, SEEK_SET);
    return nbEntries;
}

void readStructure(options_t *options, structureBase_t *structureBase,
                   int16_t precAuthornb) {
    int exitCode;
    initStructure(structureBase, precAuthornb);
    exitCode =
        fread(structureBase, 2 * sizeof(int16_t), 1, options->outputFile);
    exitCode = fread(&structureBase->title, structureBase->titleLength + 1, 1,
                     options->outputFile);
    exitCode = fread(&structureBase->authornb, sizeof(int16_t), 1,
                     options->outputFile);
    exitCode =
        fread(&structureBase->authorlengths,
              structureBase->authornb * sizeof(int8_t), 1, options->outputFile);
    for (int m = 0; m < structureBase->authornb; m++) {
        exitCode =
            fread(structureBase->author[m], structureBase->authorlengths[m] + 1,
                  1, options->outputFile);
    }
    if (exitCode == 0) {
        return;
    }
    // printStruct(structureBase);
}

void printStruct(structureBase_t *structureBase) {
    printf("Title : %s\nYear : %i\n", structureBase->title,
           structureBase->year);
    for (int r = 0; r < structureBase->authornb; r++) {
        printf("Author %i : %s\n", r, structureBase->author[r]);
    }
    printf("\n---\n\n");
}

error_t readEntireBin(options_t *options) {
    printf("\n");
    fseek(options->outputFile, 28, SEEK_SET);
    int16_t precAuthornb = 0;
    structureBase_t structureBase;
    while (1) {
        readStructure(options, &structureBase, precAuthornb);
        if (structureBase.authornb == 0 || interruptFlag == 1)
            break;
        precAuthornb = structureBase.authornb;
        printStruct(&structureBase);
    }
    return OK;
}
