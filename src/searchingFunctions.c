#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/signal.h>

#include "baseParser.h"
#include "fonctionsMatrice.h"
#include "fonctionsMatricesHash.h"
#include "io-utils.h"
#include "readFunctions.h"
#include "searchingFunctions.h"

extern int interruptFlag;

int showArticles(options_t *options) {
    initSigaction();
    /* showAuthors(options);
    fseek(options->outputFile, 0, SEEK_SET); */
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
    options->action[ACTION_SHOW_ARTICLES] = DONE_SUCCESSFULLY;
    return OK;
}

int showAuthors(options_t *options, node **hashTable) {
    initSigaction();
    fseek(options->outputFile, 0, SEEK_SET);
    int nbEntries = readEntireBin(options, 0);
    int counter = 0;
    int *hashTablePositions = calloc(50000000, sizeof(int));
    if (hashTablePositions == NULL)
        return 4;
    int16_t precAuthornb = 0;
    structureBase_t structureBase;
    unsigned int authorHash = 0;
    printf("Searching...\n\n");
    while (1) {
        initStructure(&structureBase, precAuthornb);
        structureBase = readEntryBin(options, -1);
        // printStruct(&structureBase);
        if (structureBase.authornb == 0)
            break;
        for (int k = 0; k < structureBase.authornb; k++) {
            if (strstr(structureBase.author[k], options->authorNames[0])) {
                authorHash = hash((unsigned char *)structureBase.author[k]);
                /* if (hashTablePositions[authorHash] == 0) {
                    printf(" - %s\n", hashTable[authorHash]->author);
                } */
                hashTablePositions[authorHash] = 1;
            }
        }
        if (interruptFlag == 1)
            break;
        precAuthornb = structureBase.authornb;
        counter++;
        if (counter % 50000 == 0) {
            progressBar(counter * 100 / nbEntries);
        }
    }
    progressBar(100);
    printf("\n\n");
    printf("Authors containing \"%s\" in their name : \n",
           options->authorNames[0]);
    for (unsigned int s = 0; s < 50000000; s++) {
        if (hashTablePositions[s] == 1)
            printf(" - %s\n", hashTable[s]->author);
    }
    if (options->action[ACTION_SHOW_AUTHORS] == 1)
        free(hashTablePositions);
    options->action[ACTION_SHOW_AUTHORS] = DONE_SUCCESSFULLY;
    return OK;
}