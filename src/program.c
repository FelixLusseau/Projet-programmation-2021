#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "analyseGraph.h"
#include "argsParser.h"
#include "baseParser.h"
#include "io-utils.h"
#include "makeGraph.h"
#include "program.h"
#include "readFunctions.h"
#include "searchingFunctions.h"
#include "tps_unit_test.h"

TEST_INIT_GLOBAL

#define testExitCode(op)                                                       \
    {                                                                          \
        if ((exitCode = op))                                                   \
            goto error;                                                        \
    }
#define testCorrectHash(op)                                                    \
    {                                                                          \
        if ((op) == NULL) {                                                    \
            exitCode = ERROR_VERIFY_AUTHOR;                                    \
            goto error;                                                        \
        }                                                                      \
    }

int interruptFlag = 0;

int main(int argc, char **argv) {
    int taille = 0;
    int exitCode = OK;
    initSigaction();
    options_t options;
    node *node0 = NULL;
    node **hashTable = malloc(HT_SIZE * sizeof(char *));
    if (hashTable == NULL) {
        exitCode = ERROR_HASHTABLE;
        goto error;
    }
    for (int i = 0; i < HT_SIZE; i++)
        hashTable[i] = NULL;
    testExitCode(parseArgs(argc, argv, &options));
    /* if (openFiles(&options, "r", 1) == ERROR_OPEN_BIN) {
        testExitCode(openFiles(&options, "w", 0));
        testExitCode(parseBase(&options));
        printf("\33[0;32mDatabase parsing ok ! \33[0m\n");
    } else
        closeFiles(&options);
    testExitCode(openFiles(&options, "r", 0)); */
    if (options.action[ACTION_PARSE] == TO_DO) {
        testExitCode(openFiles(&options, "w", 0));
        testExitCode(parseBase(&options));
        if (exitCode == OK && interruptFlag == 0)
            printf("\33[0;32mDatabase parsing ok ! \33[0m\n");
    }
    if (options.action[ACTION_READ] == TO_DO) {
        testExitCode(openFiles(&options, "r", 0));
        testExitCode(readEntireBin(&options, 1));
    }
    if (options.action[ACTION_GRAPH] == TO_DO) {
        testExitCode(openFiles(&options, "r", 0));
        node0 = DoListAdjDeBinHash(&options, &taille, hashTable);
        if (node0 == NULL) {
            exitCode = ERROR_LIST;
            goto error;
        }
    }
    if (options.action[ACTION_PRINT_GRAPH] == TO_DO) {
        printListAdj(node0);
        // printListNode(node0);
        // printListEdge(node0);
    }
    if (options.action[ACTION_SHOW_AUTHORS] == TO_DO) {
        showAuthors(&options, node0, 0);
    }
    if (options.action[ACTION_SHOW_ARTICLES] == TO_DO &&
        options.action[ACTION_NEIGHBOURS] != TO_DO) {
        testExitCode(showArticles(&options, node0, 0));
    }
    if (options.action[ACTION_SHOW_ARTICLES_YEAR] == TO_DO) {
        testExitCode(showArticles(&options, node0, options.year));
    }
    if (options.action[ACTION_SHORTEST_PATH] == TO_DO) {
        testExitCode(chooseAuthor(&options, node0, 0));
        testExitCode(chooseAuthor(&options, node0, 1));
        node *node1;
        testCorrectHash(node1 = verifyAuthorHash(&options, hashTable, 0));
        node *node2;
        testCorrectHash(node2 = verifyAuthorHash(&options, hashTable, 1));
        testExitCode(plusCourtChemin(node1, node2, taille));
    }
    if (options.action[ACTION_DIJKSTRA] == TO_DO) {
        testExitCode(chooseAuthor(&options, node0, 0));
        node *node1;
        testCorrectHash(node1 = verifyAuthorHash(&options, hashTable, 0));
        node *node2 = NULL;
        if (options.authorNames[1] != NULL) {
            testExitCode(chooseAuthor(&options, node0, 1));
            testCorrectHash(node2 = verifyAuthorHash(&options, hashTable, 1));
        }
        testExitCode(dijkstra(node1, node2, taille));
    }
    if (options.action[ACTION_DISTANCE] == TO_DO) {
        printDistances(&options, node0);
    }
    if (options.action[ACTION_NEIGHBOURS] == TO_DO) {
        testExitCode(printAuthorAtDist(&options, node0));
    }
    if (options.action[ACTION_CONNECTED] == TO_DO) {
        nbrComposanteConnexe(node0);
    }
    if (options.action[ACTION_UNKNOWN] == 1) {
        fprintf(stderr, "Action is missing !\n");
        printUsage();
        exitCode = ERROR_MISSING_ACTION;
        goto error;
    }
    endOfProgram(&options, node0, hashTable);

error:
    if (exitCode) {
        endOfProgram(&options, node0, hashTable);
        fprintf(stderr, "%s\n", errorToString(exitCode));
        return exitCode;
    }
    return OK;
}
