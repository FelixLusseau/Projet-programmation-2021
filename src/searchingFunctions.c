#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/signal.h>

#include "baseParser.h"
#include "fonctionsMatricesHash.h"
#include "io-utils.h"
#include "readBinary.h"
#include "searchingFunctions.h"

extern int interruptFlag;

int showArticles(options_t *options) {
    initSigaction();
    int16_t precAuthornb = 0;
    int authorWritten = 0;
    structureBase_t structureBase;
    printf("Articles of ");
    while (1) {
        initStructure(&structureBase, precAuthornb);
        structureBase = readEntryBin(options, -1);
        // printStruct(&structureBase);
        if (structureBase.authornb == 0 || interruptFlag == 1)
            break;
        for (int k = 0; k < structureBase.authornb; k++) {
            if (strstr(structureBase.author[k], options->authorNames[0])) {
                if (authorWritten == 0) {
                    printf("%s : \n", structureBase.author[k]);
                    authorWritten = 1;
                }
                printf(" - %s\n", structureBase.title);
            }
        }
        precAuthornb = structureBase.authornb;
    }
    return OK;
}

int showAuthors(options_t *options) {
    initSigaction();
    int *hashTable = malloc(50000000 * sizeof(unsigned int) * sizeof(char *));
    if (hashTable == NULL)
        return 4;
    for (int i = 0; i < 50000000; i++)
        hashTable[i] = 0;
    int16_t precAuthornb = 0;
    structureBase_t structureBase;
    unsigned int authorHash = 0;
    printf("Authors containing \"%s\" in their name : \n",
           options->authorNames[0]);
    while (1) {
        initStructure(&structureBase, precAuthornb);
        structureBase = readEntryBin(options, -1);
        // printStruct(&structureBase);
        if (structureBase.authornb == 0)
            break;
        for (int k = 0; k < structureBase.authornb; k++) {
            if (strstr(structureBase.author[k], options->authorNames[0])) {
                authorHash = hash((unsigned char *)structureBase.author[k]);
                if (hashTable[authorHash] == 0)
                    printf(" - %s\n", structureBase.author[k]);
                hashTable[authorHash] = 1;
            }
        }
        if (interruptFlag == 1)
            break;
        precAuthornb = structureBase.authornb;
    }
    return OK;
}