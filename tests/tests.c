#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "argsParser.h"
#include "baseParser.h"
#include "fonctionsMatrice.h"
#include "fonctionsMatricesHash.h"
#include "io-utils.h"
#include "program.h"
#include "readFunctions.h"
#include "searchingFunctions.h"
#include "tps_unit_test.h"

#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))
#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))

int interruptFlag = 0;

unsigned int showAllAuthors(options_t *options) {
    initSigaction();
    int16_t precAuthornb = 0;
    structureBase_t structureBase;
    /*  unsigned int max = 0;
     unsigned int min = hash((unsigned char *)structureBase.author[0]); */
    while (1) {
        initStructure(&structureBase, precAuthornb);
        structureBase = readEntryBin(options, -1);
        // printStruct(&structureBase);
        if (structureBase.authornb == 0)
            break;
        for (int k = 0; k < structureBase.authornb; k++) {
            printf(" - %s\n", structureBase.author[k]);
            /* hash((unsigned char *)structureBase.author[k]));
     printf("%lu\n", hash((unsigned char *)structureBase.author[k]));
     max = MAX(max, hash((unsigned char *)structureBase.author[k]));
     min = MIN(min, hash((unsigned char *)structureBase.author[k])); */
        }
        if (interruptFlag == 1)
            break;
        precAuthornb = structureBase.authornb;
    }
    /* printf("max : %u\n", max);
    printf("min : %u\n", min); */

    return OK;
}

void testCreateListeAdj(void) {
    char c0[] = "author0";

    node *node0 = CreateListAdj(c0);

    char *c1 = "1";
    char *c2 = "2";
    char *c3 = "3";
    char *c4 = "4";
    char *c5 = "5";
    char *c6 = "isolé";
    node *end = node0;
    end = appendNode(c1, end);
    end = appendNode(c2, end);
    end = appendNode(c3, end);
    end = appendNode(c4, end);
    end = appendNode(c5, end);
    end = appendNode(c6, end);

    appendEdge(5, 0, node0);
    appendEdge(5, 4, node0);
    appendEdge(5, 1, node0);
    appendEdge(1, 4, node0);
    appendEdge(4, 3, node0);
    appendEdge(3, 2, node0);

    printListNode(node0);
    printListEdge(node0);

    char author[] = "3";
    int test = AuthorInList(author, node0);
    printf("test présence author: %i\n", test);

    char author2[] = "z";
    int test2 = AuthorInList(author2, node0);
    printf("test présence author: %i\n", test2);

    /*dijkstra(6, node0, 6);
    printDistance(6, node0);*/
    plusCourtChemin(2,0,node0,6);
    //int rs=nbrComposanteConnexe(node0);
    //printf("nbr Connexe:%i\n",rs);

    freeListAdj(node0);
}

TEST_INIT_GLOBAL

void testParse() {
    options_t options;
    initOptions(&options);
    options.inputFilename = "sample.xml";
    options.outputFilename = "outsampletest.bin";
    options.action[ACTION_PARSE] = 1;
    openFiles(&options, "w", 0);
    tps_assert(options.inputFile != NULL);
    tps_assert(options.outputFile != NULL);
    tps_assert(parseBase(&options) == OK);
    closeFiles(&options);
}

void testRead() {
    options_t options;
    initOptions(&options);
    options.outputFilename = "outsampletest.bin";
    options.action[ACTION_PARSE] = 0;
    openFiles(&options, "r", 0);
    tps_assert(readEntireBin(&options, 1) == OK);
    closeFiles(&options);
}

void testGraph() {
    int taille = 0;
    node *node0 = NULL;
    node **hashTable = malloc(HT_SIZE * sizeof(unsigned int) * sizeof(char *));
    tps_assert(hashTable != NULL);
    for (int i = 0; i < HT_SIZE; i++)
        hashTable[i] = NULL;
    options_t options;
    initOptions(&options);
    options.outputFilename = "outsampletest.bin";
    options.action[ACTION_PARSE] = 0;
    openFiles(&options, "r", 0);
    node0 = DoListAdjDeBinHash(&options, &taille, hashTable);
    tps_assert(node0 != NULL);
    endOfProgram(&options, node0, hashTable);
}

void testArticles() {
    int taille = 0;
    node *node0 = NULL;
    node **hashTable = malloc(HT_SIZE * sizeof(unsigned int) * sizeof(char *));
    tps_assert(hashTable != NULL);
    for (int i = 0; i < HT_SIZE; i++)
        hashTable[i] = NULL;
    options_t options;
    initOptions(&options);
    options.outputFilename = "outsampletest.bin";
    options.action[ACTION_PARSE] = 0;
    options.authorNames[0] = "Russell Turpin";
    openFiles(&options, "r", 0);
    node0 = DoListAdjDeBinHash(&options, &taille, hashTable);
    tps_assert(node0 != NULL);
    tps_assert(showArticles(&options, hashTable, node0) == OK);
    endOfProgram(&options, node0, hashTable);
}

int main(void) {
    testCreateListeAdj();/*
    options_t options;
    options.inputFilename = "../database/dblp.xml";
    options.outputFilename = "../database/dblp.bin";
    options.inputFile = NULL;
    options.outputFile = NULL;
    options.action[ACTION_UNKNOWN] = 1;
    for (int a = 1; a < 6; a++)
        options.action[a] = NOT_TO_DO;
    options.authorNames[0] = NULL;
    options.authorNames[1] = NULL;

    openFiles(&options, "r", 0);
    closeFiles(&options); */
    /*
    TEST(testParse);
    TEST(testRead);
    TEST(testGraph);
    TEST(testArticles);
    */
    // showAllAuthors(&options);

    return 0;
}
