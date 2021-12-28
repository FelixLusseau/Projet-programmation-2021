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

int showArticles(options_t *options, node **hashTable, node *node0) {
    initSigaction();
    int count = showAuthorsGraph(options, hashTable, node0, 0);
    // printf("%i\n", count);
    char *exitCode = "Ok";
    fseek(options->outputFile, 0, SEEK_SET);
    if (count == 0) {
        return OK;
    } else if (count != 1) {
        printf("\n%i authors found ! Which one do you want to browse the "
               "articles ?\n"
               "Enter his complete name : ",
               count);
        // options->authorNames[0] = NULL;
        exitCode = fgets(options->authorNames[0], 100, stdin);
        if (exitCode == NULL) {
            return ERROR_SHOW_ARTICLES;
        }
        // printf("%s\n", options->authorNames[0]);
        // printf("%zu\n", strlen(options->authorNames[0]));

        options->authorNames[0][strlen(options->authorNames[0]) - 1] = '\0';
        // printf("%s\n", options->authorNames[0]);
        // printf("%zu\n", strlen(options->authorNames[0]));
    }
    int16_t precAuthornb = 0;
    structureBase_t structureBase;
    // return 2;
    printf("\nArticles of %s : \n", options->authorNames[0]);
    while (1) {
        initStructure(&structureBase, precAuthornb);
        structureBase = readEntryBin(options, -1);
        // printStruct(&structureBase);
        if (structureBase.authornb == 0 || interruptFlag == 1)
            break;
        for (int k = 0; k < structureBase.authornb; k++) {
            if (strstr(structureBase.author[k], options->authorNames[0])) {
                printf(" - %s\n", structureBase.title);
                break;
            }
        }
        precAuthornb = structureBase.authornb;
    }
    printf("\n\33[0;32mComplete ! \33[0m\n");
    options->action[ACTION_SHOW_ARTICLES] = DONE_SUCCESSFULLY;
    return OK;
}

int showAuthors(options_t *options, node **hashTable, int author0or1) {
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
            if (strstr(structureBase.author[k],
                       options->authorNames[author0or1])) {
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
    // if (options->action[ACTION_SHOW_AUTHORS] == 1)
    free(hashTablePositions);
    options->action[ACTION_SHOW_AUTHORS] = DONE_SUCCESSFULLY;
    return OK;
}

int showAuthorsGraph(options_t *options, node **hashTable, node *node0,
                     int author0or1) {
    initSigaction();
    fseek(options->outputFile, 0, SEEK_SET);
    if (node0 == NULL) {
        return 4;
    }
    node *currentNode = node0;
    char *authortmp = currentNode->author;
    int counter = 0;
    int *hashTablePositions = calloc(50000000, sizeof(int));
    if (hashTablePositions == NULL)
        return 7;
    unsigned int authorHash = 0;
    printf("Searching...\n\n");
    while (currentNode->nextNode != NULL) {
        if (strstr(currentNode->author, options->authorNames[author0or1])) {
            authortmp = currentNode->author;
            authorHash = hash((unsigned char *)currentNode->author);
            if (hashTablePositions[authorHash] == 0) {
                // printf(" - %s\n", hashTable[authorHash]->author);
                counter++;
            }
            hashTablePositions[authorHash] = 1;
        }
        currentNode = currentNode->nextNode;
        if (interruptFlag == 1)
            break;
    }
    if (counter != 0) {
        printf("Authors containing \"%s\" in their name : \n",
               options->authorNames[author0or1]);
        for (unsigned int s = 0; s < 50000000; s++) {
            if (hashTablePositions[s] == 1) {
                printf(" - %s\n", hashTable[s]->author);
            }
        }
    } else {
        printf("No author containing \"\33[0;31m%s\33[0m\" in his name ! \n",
               options->authorNames[author0or1]);
    }
    if (counter == 1) {
        options->authorNames[author0or1] = authortmp;
    }
    free(hashTablePositions);
    options->action[ACTION_SHOW_AUTHORS] = DONE_SUCCESSFULLY;
    return counter;
}
