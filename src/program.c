#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "argsParser.h"
#include "baseParser.h"
#include "fonctionsMatrice.h"
#include "fonctionsMatricesHash.h"
#include "io-utils.h"
#include "program.h"
#include "readBinary.h"
#include "searchingFunctions.h"

int interruptFlag = 0;

int main(int argc, char **argv) {
    int taille = 0;
    initSigaction();
    int exitCode;
    options_t options;
    node *node0;
    exitCode = parseArgs(argc, argv, &options);
    if (exitCode)
        goto error;
    /* A activer plus tard */
    /* if (openFiles(&options, "r", 1) == ERROR_OPEN_BIN) {
        exitCode = openFiles(&options, "w", 0);
        if (exitCode)
            goto error;
        exitCode = parseBase(&options);
        if (exitCode)
            goto error;
        printf("Database parsing ok ! \n");
    } else
        closeFiles(&options); */
    switch (options.action) {
    case ACTION_PARSE:
        exitCode = openFiles(&options, "w", 0);
        if (exitCode)
            goto error;
        exitCode = parseBase(&options);
        if (exitCode)
            goto error;
        printf("\33[0;32mDatabase parsing ok ! \33[0m\n");
        break;
    case ACTION_READ:
        exitCode = openFiles(&options, "r", 0);
        if (exitCode)
            goto error;
        exitCode = readEntireBin(&options);
        if (exitCode)
            goto error;
        break;
    case ACTION_MAT:
        exitCode = openFiles(&options, "r", 0);
        node0 = DoListAdjDeBinHash(&options, &taille);
        freeListAdj(node0);
        break;
    case ACTION_SHOW_ARTICLES:
        exitCode = openFiles(&options, "r", 0);
        if (exitCode)
            goto error;
        exitCode = showArticles(&options);
        if (exitCode)
            goto error;
        break;
    case ACTION_SHOW_AUTHORS:
        exitCode = openFiles(&options, "r", 0);
        if (exitCode)
            goto error;
        exitCode = showAuthors(&options);
        if (exitCode)
            goto error;
        break;
    default:
        fprintf(stderr, "Action is missing !\n");
        printUsage();
        exitCode = ERROR_MISSING_ACTION;
        goto error;
        break;
    }
    closeFiles(&options);

error:
    if (exitCode) {
        fprintf(stderr, "%s\n", errorToString(exitCode));
        return exitCode;
    }
    return OK;
}
