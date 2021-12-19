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

int interruptFlag = 0;

int main(int argc, char **argv) {
    int taille = 0;
    initSigaction();
    int exitCode;
    options_t options;
    node *node0 = NULL;
    exitCode = parseArgs(argc, argv, &options);
    if (exitCode)
        goto error;
    /* if (openFiles(&options, "r", 1) == ERROR_OPEN_BIN) {
        exitCode = openFiles(&options, "w", 0);
        if (exitCode)
            goto error;
        exitCode = parseBase(&options);
        if (exitCode)
            goto error;
        printf("\33[0;32mDatabase parsing ok ! \33[0m\n");
    } else
        closeFiles(&options);
    exitCode = openFiles(&options, "r", 0); */
    node **hashTable = malloc(50000000 * sizeof(unsigned int) * sizeof(char *));
    if (hashTable == NULL)
        return ERROR_GRAPH;
    for (int i = 0; i < 50000000; i++)
        hashTable[i] = NULL;
    if (options.action[ACTION_PARSE] == TO_DO) {
        exitCode = openFiles(&options, "w", 0);
        if (exitCode)
            goto error;
        exitCode = parseBase(&options);
        if (exitCode)
            goto error;
        printf("\33[0;32mDatabase parsing ok ! \33[0m\n");
    }
    if (options.action[ACTION_READ] == TO_DO) {
        exitCode = openFiles(&options, "r", 0);
        if (exitCode)
            goto error;
        exitCode = readEntireBin(&options, 1);
        if (exitCode)
            goto error;
    }
    if (options.action[ACTION_MAT] == TO_DO) {
        exitCode = openFiles(&options, "r", 0);
        node0 = DoListAdjDeBinHash(&options, &taille, hashTable);
        // printListNode(node0);
        /* for (int s = 0; s < 50000000; s++)
            printf("%s\n", &(*hashTable[s]).author); */
    }
    if (options.action[ACTION_SHOW_ARTICLES] == TO_DO) {
        exitCode = showArticles(&options);
        if (exitCode)
            goto error;
    }
    if (options.action[ACTION_SHOW_AUTHORS] == TO_DO) {
        exitCode = showAuthors(&options, hashTable);
        if (exitCode)
            goto error;
    }
    if (options.action[ACTION_UNKNOWN] == 1) {
        fprintf(stderr, "Action is missing !\n");
        printUsage();
        exitCode = ERROR_MISSING_ACTION;
        goto error;
    }
    if (node0 != NULL)
        freeListAdj(node0);
    free(hashTable);
    closeFiles(&options);

error:
    if (exitCode) {
        fprintf(stderr, "%s\n", errorToString(exitCode));
        return exitCode;
    }
    return OK;
}
