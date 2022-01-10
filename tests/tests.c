#include <math.h>
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

/* void testCreateListeAdj(void) {
    char c0[] = "g";

    node *node0 = CreateListAdj(c0);

    char *c1 = "a";
    char *c2 = "b";
    char *c3 = "c";
    char *c4 = "d";
    char *c5 = "e";
    char *c6 = "f";
    node *end = node0;
    end = appendNode(c1, end);
    end = appendNode(c2, end);
    end = appendNode(c3, end);
    end = appendNode(c4, end);
    end = appendNode(c5, end);
    end = appendNode(c6, end);

    appendEdge(1, 2, node0);
    appendEdge(1, 3, node0);
    appendEdge(1, 5, node0);
    appendEdge(2, 4, node0);
    appendEdge(2, 6, node0);
    appendEdge(3, 0, node0);
    appendEdge(5, 6, node0);

    printListNode(node0);
    printListEdge(node0);

    char author[] = "d";
    int test = AuthorInList(author, node0);
    printf("test présence author: %i\n", test);

    char author2[] = "z";
    int test2 = AuthorInList(author2, node0);
    printf("test présence author: %i\n", test2);

    //options_t options;
    //initOptions(&options);
    //options.authorNames[0] = "a";
    //dijkstra(1, node0, 6);
    //printDistances(&options, node0);
    // plusCourtChemin(3,2,node0,6);
    // int rs=nbrComposanteConnexe(node0);
    // printf("nbr Connexe:%i\n",rs);

    freeListAdj(node0, 1);
} */

TEST_INIT_GLOBAL

void testParseArgs() {
    options_t options;
    int argc = 22;
    char *argv[] = {"./bin/program",
                    "-i",
                    "tests/sample.xml",
                    "-o",
                    "tests/sample.bin",
                    "-x",
                    "-r",
                    "-m",
                    "-a",
                    "Quentin Bramas",
                    "-y",
                    "2021",
                    "-l",
                    "Pascal M&eacute;rindol",
                    "-p",
                    "Quentin Bramas",
                    "-p",
                    "Pascal M&eacute;rindol",
                    "-d",
                    "-n",
                    "2",
                    "-c"};
    parseArgs(argc, argv, &options);
    tps_assert(strcmp(options.outputFilename, "tests/sample.bin") == 0);
    tps_assert(options.year == 2021);
    tps_assert(strcmp(options.authorNames[0], "Pascal M&eacute;rindol") == 0);
    tps_assert(options.action[ACTION_SHOW_ARTICLES_YEAR] == TO_DO);
}

void testArgMissing() {
    options_t options;
    int argc = 3;
    char *argv[] = {"./bin/program", "-o", "-m"};
    tps_assert(parseArgs(argc, argv, &options) == ERROR_ARGS_PARSE);
}

void testParseBase() {
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

void testMallocError() {
    __remaining_alloc = 0;
    int exitCode = OK;
    options_t options;
    initOptions(&options);
    options.inputFilename = "sample.xml";
    options.outputFilename = "out";
    options.action[ACTION_PARSE] = 1;
    openFiles(&options, "w", 0);
    tps_assert((exitCode = parseBase(&options)) == ERROR_BASE_PARSE);
    fprintf(stderr, "%s\n", errorToString(exitCode));
    closeFiles(&options);
    __remaining_alloc = -1;
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
    printListAdj(node0);
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
    tps_assert(showArticles(&options, node0, 0) == OK);
    endOfProgram(&options, node0, hashTable);
}

void testConnected() {
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
    options.action[ACTION_GRAPH] = TO_DO;
    options.action[ACTION_CONNECTED] = TO_DO;
    openFiles(&options, "r", 0);
    node0 = DoListAdjDeBinHash(&options, &taille, hashTable);
    tps_assert(node0 != NULL);
    nbrComposanteConnexe(node0);
    tps_assert(node0->flag == 1);
    endOfProgram(&options, node0, hashTable);
}

int main(void) {
    /*// testCreateListeAdj();
    options_t options;
    options.inputFilename = "../database/dblp.xml";
    // options.outputFilename = "../database/dblp.bin";
    // options.outputFilename = "../tests/sample.bin";
    options.inputFile = NULL;
    options.outputFile = NULL;
    options.action[ACTION_UNKNOWN] = 1;
    for (int a = 1; a < 6; a++)
        options.action[a] = NOT_TO_DO;
    options.authorNames[0] = NULL;
    options.authorNames[1] = NULL;

    openFiles(&options, "r", 0);

    closeFiles(&options);*/

    TEST(testParseArgs);
    // TEST(testArgMissing);
    TEST(testParseBase);
    TEST(testMallocError);
    TEST(testRead);
    TEST(testGraph);
    TEST(testArticles);
    TEST(testConnected);

    return 0;
}
