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

int showAuthors(options_t *options, node **hashTable, node *node0,
                int author0or1) {
    initSigaction();
    fseek(options->outputFile, 14, SEEK_SET);
    if (node0 == NULL) {
        return ERROR_NODE_EQ_NULL;
    }
    node *currentNode = node0;
    char *authortmp = currentNode->author;
    int counter = 0;
    int *hashTablePositions = calloc(HT_SIZE, sizeof(int));
    if (hashTablePositions == NULL)
        return ERROR_SHOW_AUTHORS;
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
    /* Take the last author */
    if (strstr(currentNode->author, options->authorNames[author0or1])) {
        authortmp = currentNode->author;
        authorHash = hash((unsigned char *)currentNode->author);
        if (hashTablePositions[authorHash] == 0) {
            // printf(" - %s\n", hashTable[authorHash]->author);
            counter++;
        }
        hashTablePositions[authorHash] = 1;
    }
    if (counter != 0) {
        printf("Authors containing \"%s\" in their name : \n",
               options->authorNames[author0or1]);
        for (unsigned int s = 0; s < HT_SIZE; s++) {
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
    printf("\n\n");
    return counter;
}

int chooseAuthor(options_t *options, node **hashTable, node *node0,
                 int author0or1) {
    int count = showAuthors(options, hashTable, node0, author0or1);
    fseek(options->outputFile, 14, SEEK_SET);
    char *exitChar = "Ok";
    if (count == 0) {
        return ERROR_NO_AUTHOR;
    } else if (count != 1) {
        printf("\n%i authors found ! Which one do you want to see ?\n"
               "Enter his complete name : ",
               count);
        exitChar = fgets(options->authorNames[0], 100, stdin);
        if (exitChar == NULL) {
            return ERROR_SHOW_ARTICLES;
        }
        options->authorNames[0][strlen(options->authorNames[0]) - 1] = '\0';
    }
    return OK;
}

int showArticles(options_t *options, node **hashTable, node *node0, int year) {
    initSigaction();
    int exitCode = chooseAuthor(options, hashTable, node0, 0);
    if (exitCode == ERROR_SHOW_ARTICLES)
        return ERROR_SHOW_ARTICLES;
    else if (exitCode == ERROR_NO_AUTHOR) {
        return OK;
    }
    int count = 0;
    int16_t precAuthornb = 0;
    structureBase_t structureBase;
    printf("\nArticles of %s", options->authorNames[0]);
    if (year > 0) {
        printf(" in %i", year);
    }
    printf(" : \n");
    while (1) {
        initStructure(&structureBase, precAuthornb);
        structureBase = readEntryBin(options, -1);
        if (structureBase.authornb == 0 || interruptFlag == 1)
            break;
        for (int k = 0; k < structureBase.authornb; k++) {
            if (strstr(structureBase.author[k], options->authorNames[0])) {
                if ((year > 0 && structureBase.year == year) || year == 0)
                    printf(" - %s\n", structureBase.title);
                count++;
                break;
            }
        }
        precAuthornb = structureBase.authornb;
    }
    if (count == 0 && year == 0) {
        fprintf(stderr, "\nInvalid author name given ! \n");
        return ERROR_SHOW_ARTICLES;
    }
    if (count == 0 && year != 0) {
        fprintf(stderr, "\nInvalid author name or year given ! \n");
        return ERROR_SHOW_ARTICLES;
    }
    if (interruptFlag != 1)
        printf("\n\n\33[0;32mComplete ! \33[0m\n");
    return OK;
}