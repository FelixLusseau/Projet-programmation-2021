#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "argsParser.h"
#include "baseParser.h"
#include "fonctionsMatrice.h"
#include "fonctionsMatricesHash.h"
#include "io-utils.h"
#include "program.h"
#include "readFunctions.h"
#include "searchingFunctions.h"

#define testExitCode(op)                                                       \
    {                                                                          \
        if ((exitCode = op))                                                   \
            goto error;                                                        \
    }

int interruptFlag = 0;

int main(int argc, char **argv) {
    int taille = 0;
    int exitCode = OK;
    initSigaction();
    options_t options;
    node *node0 = NULL;
    node **hashTable = malloc(HT_SIZE * sizeof(unsigned int) * sizeof(char *));
    if (hashTable == NULL) {
        exitCode = ERROR_GRAPH;
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
        printf("\33[0;32mDatabase parsing ok ! \33[0m\n");
    }
    if (options.action[ACTION_READ] == TO_DO) {
        testExitCode(openFiles(&options, "r", 0));
        testExitCode(readEntireBin(&options, 1));
    }
    if (options.action[ACTION_GRAPH] == TO_DO) {
        testExitCode(openFiles(&options, "r", 0));
        node0 = DoListAdjDeBinHash(&options, &taille, hashTable);
        printListNode(node0);
        // printListAdj(node0);
        // printf("%i\n", authorNameToNodeNumber("Russell Turpin", hashTable));
        // printf("%i\n", authorNameToNodeNumber("Dimitar Ruscev", hashTable));
    }
    if (options.action[ACTION_SHOW_AUTHORS] == TO_DO) {
        showAuthors(&options, hashTable, node0, 0);
    }
    if (options.action[ACTION_SHOW_ARTICLES] == TO_DO &&
        options.action[ACTION_NEIGHBOURS] != TO_DO) {
        testExitCode(showArticles(&options, hashTable, node0));
    }
    if (options.action[ACTION_SHORTEST_PATH] == TO_DO) {
        testExitCode(chooseAuthor(&options, hashTable, node0, 0));
        testExitCode(chooseAuthor(&options, hashTable, node0, 1));
        testExitCode(plusCourtChemin(
            authorNameToNodeNumber(options.authorNames[0], hashTable),
            authorNameToNodeNumber(options.authorNames[1], hashTable), node0,
            taille));
    }
    if (options.action[ACTION_DIJKSTRA] == TO_DO) {
        testExitCode(chooseAuthor(&options, hashTable, node0, 0));
        testExitCode(
            dijkstra(authorNameToNodeNumber(options.authorNames[0], hashTable),
                     node0, taille));
    }
    if (options.action[ACTION_DISTANCE] == TO_DO) {
        testExitCode(chooseAuthor(&options, hashTable, node0, 1));
        printDistance(authorNameToNodeNumber(options.authorNames[1], hashTable),
                      node0);
    }
    if (options.action[ACTION_NEIGHBOURS] == TO_DO) {
        testExitCode(printAuthorAtDist(&options, node0));
    }
    if (options.action[ACTION_CONNECTED] == TO_DO) {
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
