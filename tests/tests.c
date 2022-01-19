#include <math.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "analyseGraph.h"
#include "argsParser.h"
#include "baseParser.h"
#include "io-utils.h"
#include "makeGraph.h"
#include "program.h"
#include "readBin.h"
#include "searchingFunctions.h"
#include "tps_unit_test.h"

int interruptFlag = 0;

TEST_INIT_GLOBAL

void testArgMissing() {
    options_t options;
    int argc = 3;
    char *argv[] = {"./bin/program", "-g", "-o"};
    tps_assert(parseArgs(argc, argv, &options) == ERROR_ARGS_PARSE);
}

void testErrorFiles() {
    options_t options;
    initOptions(&options);
    int exitCode;
    options.inputFilename = "sampl.xml";
    options.action[ACTION_PARSE] = TO_DO;
    tps_assert((exitCode = openFiles(&options, "w")) == ERROR_OPEN_DATABASE);
    closeFiles(&options);
    fprintf(stderr, "%s\n", errorToString(exitCode));
    options.inputFilename = "sample.bin";
    tps_assert((exitCode = openFiles(&options, "w")) == ERROR_XML);
    fprintf(stderr, "%s\n", errorToString(exitCode));

    options.action[ACTION_PARSE] = NOT_TO_DO;
    options.outputFilename = "out9";
    tps_assert((exitCode = openFiles(&options, "r")) == ERROR_OPEN_BIN);
    fprintf(stderr, "%s\n", errorToString(exitCode));
    options.outputFilename = "sample.xml";
    tps_assert((exitCode = openFiles(&options, "r")) == ERROR_BIN);
    closeFiles(&options);
    fprintf(stderr, "%s\n", errorToString(exitCode));
}

void testMallocError1() {
    __remaining_alloc = 0;
    int exitCode = OK;
    node **hashTable = malloc(HT_SIZE * sizeof(unsigned int) * sizeof(char *));
    tps_assert(hashTable == NULL);
    if (hashTable == NULL)
        exitCode = ERROR_HASHTABLE;
    fprintf(stderr, "%s\n", errorToString(exitCode));
    options_t options;
    initOptions(&options);
    options.inputFilename = "sample.xml";
    options.outputFilename = "out";
    options.action[ACTION_PARSE] = TO_DO;
    openFiles(&options, "w");
    tps_assert((exitCode = parseBase(&options)) == ERROR_BASE_PARSE);
    fprintf(stderr, "%s\n", errorToString(exitCode));
    closeFiles(&options);
    __remaining_alloc = -1;
}

void testMallocError2() {
    node **hashTable = malloc(HT_SIZE * sizeof(unsigned int) * sizeof(char *));
    tps_assert(hashTable != NULL);
    for (int i = 0; i < HT_SIZE; i++)
        hashTable[i] = NULL;
    __remaining_alloc = 0;
    options_t options;
    initOptions(&options);
    int exitCode = OK;
    options.outputFilename = "sample.bin";
    openFiles(&options, "r");
    node *node0;
    int size = 0;
    tps_assert((node0 = doListAdjHash(&options, &size, hashTable)) == NULL);
    if (node0 == NULL)
        exitCode = ERROR_LIST;
    fprintf(stderr, "%s\n", errorToString(exitCode));
    tps_assert((exitCode = printAuthorAtDist(&options, node0)) ==
               ERROR_NODE_EQ_NULL);
    fprintf(stderr, "%s\n", errorToString(exitCode));
    endOfProgram(&options, node0, hashTable);
    __remaining_alloc = -1;
}

void testInterruption() {
    int raison;
    node *node0 = NULL;
    node **hashTable = NULL;
    options_t options;
    initOptions(&options);
    initSigaction();
    switch (fork()) {
    case -1:
        fprintf(stderr, "Error generating the sub process\n");
        break;
    case 0:
        sleep(0.9);
        kill(getppid(), SIGINT);
        exit(0);
    default:
        options.outputFilename = "outsampletest.bin";
        openFiles(&options, "r");
        tps_assert(readEntireBin(&options, 1) == OK);
        endOfProgram(&options, node0, hashTable);
        if (wait(&raison) == -1)
            fprintf(stderr, "fail wait\n");
        if (WIFEXITED(raison))
            fprintf(stderr, "exit(%d)\n", WEXITSTATUS(raison));
        else if (WIFSIGNALED(raison))
            fprintf(stderr, "signal %d\n", WTERMSIG(raison));
        else
            fprintf(stderr, "other reason\n");
        tps_assert(interruptFlag == 1);
        break;
    }
    interruptFlag = 0;
}

void testParseArgs() {
    printUsage();
    options_t options;
    int argc = 23;
    char *argv[] = {"./bin/program",
                    "-i",
                    "tests/sample.xml",
                    "-o",
                    "tests/sample.bin",
                    "-x",
                    "-r",
                    "-g",
                    "-s",
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
    tps_assert(parseArgs(argc, argv, &options) == OK);
    tps_assert(strcmp(options.outputFilename, "tests/sample.bin") == 0);
    tps_assert(options.year == 2021);
    tps_assert(strcmp(options.authorNames[0], "Pascal M&eacute;rindol") == 0);
    tps_assert(options.action[ACTION_SHOW_ARTICLES_YEAR] == TO_DO);
}

void testParseBase() {
    options_t options;
    initOptions(&options);
    options.inputFilename = "sample.xml";
    options.outputFilename = "outsampletest.bin";
    options.action[ACTION_PARSE] = 1;
    openFiles(&options, "w");
    tps_assert(options.inputFile != NULL);
    tps_assert(options.outputFile != NULL);
    tps_assert(parseBase(&options) == OK);
    closeFiles(&options);
}

void testRead() {
    options_t options;
    initOptions(&options);
    options.outputFilename = "outsampletest.bin";
    openFiles(&options, "r");
    tps_assert(readEntireBin(&options, 1) == OK);
    closeFiles(&options);
}

void testGraph() {
    int size = 0;
    node **hashTable = malloc(HT_SIZE * sizeof(unsigned int) * sizeof(char *));
    tps_assert(hashTable != NULL);
    for (int i = 0; i < HT_SIZE; i++)
        hashTable[i] = NULL;
    options_t options;
    initOptions(&options);
    options.outputFilename = "outsampletest.bin";
    openFiles(&options, "r");
    node *node0 = doListAdjHash(&options, &size, hashTable);
    tps_assert(node0 != NULL);
    printListAdj(node0);
    endOfProgram(&options, node0, hashTable);
}

void testArticles() {
    int size = 0;
    node **hashTable = malloc(HT_SIZE * sizeof(unsigned int) * sizeof(char *));
    tps_assert(hashTable != NULL);
    for (int i = 0; i < HT_SIZE; i++)
        hashTable[i] = NULL;
    options_t options;
    initOptions(&options);
    options.outputFilename = "outsampletest.bin";
    options.authorNames[0] = "Russell Turpin";
    openFiles(&options, "r");
    node *node0 = doListAdjHash(&options, &size, hashTable);
    tps_assert(node0 != NULL);
    tps_assert(showArticles(&options, node0, 0) == OK);
    endOfProgram(&options, node0, hashTable);
}

void testShortestPath() {
    int size = 0;
    node **hashTable = malloc(HT_SIZE * sizeof(unsigned int) * sizeof(char *));
    tps_assert(hashTable != NULL);
    for (int i = 0; i < HT_SIZE; i++)
        hashTable[i] = NULL;
    options_t options;
    initOptions(&options);
    options.outputFilename = "sample2.bin";
    options.authorNames[0] = "Takaya Asano";
    options.authorNames[1] = "Takuya Iwamoto";
    openFiles(&options, "r");
    node *node0 = doListAdjHash(&options, &size, hashTable);
    tps_assert(node0 != NULL);
    node *node1 = verifyAuthorHash(&options, hashTable, 0);
    node *node2 = verifyAuthorHash(&options, hashTable, 1);
    tps_assert(shortestPath(node1, node2, size) == OK);
    tps_assert(node2->distance != 0);
    tps_assert(node2->flag == 1);
    endOfProgram(&options, node0, hashTable);
}

void testDistances() {
    int size = 0;
    node **hashTable = malloc(HT_SIZE * sizeof(unsigned int) * sizeof(char *));
    tps_assert(hashTable != NULL);
    for (int i = 0; i < HT_SIZE; i++)
        hashTable[i] = NULL;
    options_t options;
    initOptions(&options);
    options.outputFilename = "sample2.bin";
    options.authorNames[0] = "Takaya Asano";
    options.authorNames[1] = "Takuya Iwamoto";
    options.N = 2;
    openFiles(&options, "r");
    node *node0 = doListAdjHash(&options, &size, hashTable);
    tps_assert(node0 != NULL);
    node *node1 = verifyAuthorHash(&options, hashTable, 0);
    node *node2 = verifyAuthorHash(&options, hashTable, 1);
    tps_assert(dijkstra(node1, node2, size) == OK);
    tps_assert(node2->distance != 0);
    tps_assert(node2->flag == 1);
    printDistances(&options, node0);
    tps_assert(printAuthorAtDist(&options, node0) == OK);
    endOfProgram(&options, node0, hashTable);
}

void testConnected() {
    int size = 0;
    node **hashTable = malloc(HT_SIZE * sizeof(unsigned int) * sizeof(char *));
    tps_assert(hashTable != NULL);
    for (int i = 0; i < HT_SIZE; i++)
        hashTable[i] = NULL;
    options_t options;
    initOptions(&options);
    options.outputFilename = "outsampletest.bin";
    openFiles(&options, "r");
    node *node0 = doListAdjHash(&options, &size, hashTable);
    tps_assert(node0 != NULL);
    nbrConnectedComponent(node0);
    tps_assert(node0->flag == 1);
    endOfProgram(&options, node0, hashTable);
}

int main(void) {
    TEST(testErrorFiles);
    TEST(testMallocError1);
    TEST(testMallocError2);
    TEST(testArgMissing);
    TEST(testInterruption);
    TEST(testParseArgs);
    TEST(testParseBase);
    TEST(testRead);
    TEST(testGraph);
    TEST(testArticles);
    TEST(testShortestPath);
    TEST(testDistances);
    TEST(testConnected);

    return OK;
}
