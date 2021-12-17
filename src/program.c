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
        printf("Database parsing ok ! \n");
        break;
    case ACTION_READ:
        exitCode = openFiles(&options, "r", 0);
        if (exitCode)
            goto error;
        exitCode = readEntireBin(&options);
        if (exitCode)
            goto error;
        /* structureBase_t structureBase;
        //initStructure(&structureBase, 0);
        int curseur=0;
        structureBase = readEntryBin(&options, 45);
        printStruct(&structureBase);
        do{
            structureBase = readEntryBin(&options, -1);
            curseur++;
            printStruct(&structureBase);
        } while (curseur!=200); */
        break;
    case ACTION_MAT:
        exitCode = openFiles(&options, "r", 0);
        node0 = DoListAdjDeBin(&options, &taille);
        printListNode(node0);
        printListEdge(node0);
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

/* int main(int argc, char ** argv){
    options_t options;
    parseArgs(argc, argv, &options);
    openFiles(&options);
    parseBase(&options);
    structureBase_t structureBase;
    initStructure(&structureBase, 0);
    structureBase = readEntryBin(&options, 0);
    node *node0 =DoListAdjDeBin(&options);
    printListNode(node0);
    printListEdge(node0);
    freeListAdj(node0);
    closeFiles(&options);
    return 0;
}    */
