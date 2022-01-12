#include "io-utils.h"
#include <ctype.h>
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fonctionsMatrice.h"
#include "program.h"

extern int interruptFlag;

void handleSignal() {
    // int carac;
    fprintf(stdout, "\n\33[0;33mCTRL+C pressed !\33[0m\nDo you want to save "
                    "and exit ? [Y / N]\nAns : ");
    int carac = getc(stdin);
    if (carac == 'Y' || carac == 'y') {
        interruptFlag = 1;
        fprintf(stdout, "Exit !\n");
    }
    // printf("interruptFlag : %i\n", interruptFlag);
}
void initSigaction() {
    struct sigaction sa;
    sa.sa_handler = &handleSignal;
    sa.sa_flags = SA_RESTART;
    sigfillset(&sa.sa_mask);
    if (sigaction(SIGINT, &sa, NULL) == -1) {
        perror(
            "\33[0;31mError:\33[0m cannot handle SIGINT"); // Should not happen
    }
}

void initOptions(options_t *options) {
    options->inputFilename = NULL;
    options->outputFilename = NULL;
    options->inputFile = NULL;
    options->outputFile = NULL;
    options->action[ACTION_UNKNOWN] = 1;
    for (int a = 1; a < ACTIONS_NB + 1; a++)
        options->action[a] = NOT_TO_DO;
    for (int b = 0; b < 2; b++)
        options->authorNames[b] = NULL;
    options->N = 0;
    options->year = 0;
}

int isXML(FILE *file) {
    char docType[6];
    int exitCode = fread(&docType, 1, 5, file);
    docType[5] = '\0';
    if (exitCode == -1)
        return ERROR_XML;
    if (strcmp("<?xml", docType) != 0) {
        return ERROR_XML;
    }
    fseek(file, 0, SEEK_SET);
    return OK;
}

int isBinOrEmpty(FILE *file) {
    char line[20];
    char *exitString = fgets(&line[0], 20, file);
    if (exitString == NULL) {
        return ERROR_BIN;
    }
    // printf("%li", ftell(file));
    if (strcmp(line, "<binary file>\n")) {
        return ERROR_BIN;
    }
    return OK;
}

int openFiles(options_t *options, char *openMode, int test) {
    errno = 0;
    if (options->action[ACTION_PARSE] == TO_DO) {
        options->inputFile = fopen(options->inputFilename, "r");
        if (options->inputFile == NULL) {
            fprintf(stderr, "%s : %s.\n", strerror(errno),
                    options->inputFilename);
            return ERROR_OPEN_DATABASE;
        }
        if (isXML(options->inputFile) == ERROR_XML) {
            return ERROR_XML;
        }
        remove(options->outputFilename);
    }
    options->outputFile = fopen(options->outputFilename, openMode);
    if (options->outputFile == NULL) {
        if (test != 1) {
            fprintf(stderr, "%s : %s.\n", strerror(errno),
                    options->outputFilename);
        }
        return ERROR_OPEN_BIN;
    }
    if (strcmp(openMode, "r") == 0 && isBinOrEmpty(options->outputFile) != OK) {
        return ERROR_BIN;
    }
    return OK;
}

int closeFiles(options_t *options) {
    if (options->inputFile != NULL)
        fclose(options->inputFile);
    if (options->outputFile != NULL)
        fclose(options->outputFile);
    return OK;
}

void endOfProgram(options_t *options, node *node0, node **hashTable) {
    if (node0 != NULL)
        freeListAdj(node0, 1);
    free(hashTable);
    if (options->inputFile != NULL || options->outputFile != NULL)
        closeFiles(options);
    if (interruptFlag == 1)
        fprintf(stderr, "\33[0;33mInterrupted !\33[0m\n");
}

const char *errorToString(error_t err) {
    switch (err) {
    case ERROR_ARGS_PARSE:
        return "=> \33[0;31mError\33[0m while parsing arguments !";
    case ERROR_OPEN_DATABASE:
        return "=> \33[0;31mError\33[0m while opening database ! You need to "
               "give a xml file with the -i database.xml option !";
    case ERROR_OPEN_BIN:
        return "=> \33[0;31mError\33[0m while opening binary !";
    case ERROR_XML:
        return "=> The input file isn't a XML database !";
    case ERROR_BIN:
        return "=> The output file is not a binary or an empty file !";
    case ERROR_BASE_PARSE:
        return "=> \33[0;31mError\33[0m while parsing base !";
    case ERROR_READ:
        return "=> \33[0;31mError\33[0m while reading the binary !";
    case ERROR_LIST:
        return "=> \33[0;31mError\33[0m while generating the adjacence list !";
    case ERROR_SHOW_ARTICLES:
        return "=> \33[0;31mError\33[0m while showing the articles !";
    case ERROR_SHOW_AUTHORS:
        return "=> \33[0;31mError\33[0m while showing the authors !";
    case ERROR_CHOOSE_AUTHOR:
        return "=> \33[0;31mError\33[0m while choosing the author(s) !";
    case ERROR_HASHTABLE:
        return "=> \33[0;31mError\33[0m while generating the hashtable !";
    case ERROR_NODE_EQ_NULL:
        return "=> \33[0;31mError\33[0m graph is empty !";
    case ERROR_PATH:
        return "=> \33[0;31mError\33[0m making the path !";
    case ERROR_VERIFY_AUTHOR:
        return "=> \33[0;31mError\33[0m on verfying hash for the author !";
    case OK:
        return "";
    default:
        return "";
    }
}

void progressBar(int n) {
    printf("\r[");
    for (int i = 1; i <= n; i++) {
        printf("\33[0;35m#\33[0m");
    }
    for (int i = n; i < 100; i++) {
        printf(" ");
    }
    printf("]");
}