#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/signal.h>

#include "baseParser.h"
#include "io-utils.h"
#include "program.h"
#include "readFunctions.h"

extern int interruptFlag;

/* int readEntireXML(options_t *options) {
    initSigaction();
    errno = 0;
    fseek(options->inputFile, 0, SEEK_SET);
    char *line = malloc(1500);
    if (line == NULL) {
        fprintf(stderr, "%s.\n", strerror(errno));
        return ERROR_BASE_PARSE;
    }
    int counter = 0;
    while (fgets(line, 1500, options->inputFile) != NULL) {
        if (line[0] == '<' && line[1] == '/')
            counter++;
    }
    fseek(options->inputFile, 0, SEEK_SET);
    return counter;
} */

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

int readEntireBin(options_t *options, int print) {
    initSigaction();
    fseek(options->outputFile, 0, SEEK_SET);
    int counter = 0;
    int16_t precAuthornb = 0;
    structureBase_t structureBase;
    while (1) {
        readStructure(options, &structureBase, precAuthornb);
        if (structureBase.authornb == 0 || interruptFlag == 1)
            break;
        precAuthornb = structureBase.authornb;
        if (print == 1)
            printStruct(&structureBase);
        counter++;
    }
    if (print == 0) {
        fseek(options->outputFile, 0, SEEK_SET);
        return counter;
    }
    options->action[ACTION_READ] = DONE_SUCCESSFULLY;
    return OK;
}

structureBase_t readEntryBin(options_t *options, int curseur) {
    int16_t precAuthornb = 0;
    int count = 0;
    structureBase_t structureBase;
    if (curseur == -1) {
        readStructure(options, &structureBase, precAuthornb);
    } else if (curseur != 0) {
        fseek(options->outputFile, 0, SEEK_SET);
        while (count <= curseur) {
            readStructure(options, &structureBase, precAuthornb);
            count++;
            precAuthornb = structureBase.authornb;
        }
    }
    return structureBase;
}

void printStruct(structureBase_t *structureBase) {
    printf("title : %s\nyear : %i\n", structureBase->title,
           structureBase->year);
    for (int r = 0; r < structureBase->authornb; r++) {
        printf("author %i : %s\n", r, structureBase->author[r]);
    }
    printf("\n");
}