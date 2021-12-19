#include "argsParser.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void printUsage(void) {
    printf("usage: ./bin/program OPTIONS\n"
           "options:\n"
           "\t-i FICHIER               indicate the xml to read\n"
           "\t-o FICHIER               indicate the binary to write\n"
           "\t-x                       generate the binary file from the xml "
           "database\n"
           "\t-r                       read and print the binary structure for "
           "each document\n"
           "\t-m                       make the adjacence list from the "
           "binary file\n"
           "\t-a AUTHOR                show all the titles of the document "
           "where the author has participated\n"
           "\t-l WORD                  show all the authors containing WORD in "
           "their name\n"
           "\t-p AUTHOR1 -p AUTHOR2    make the graph of the shortest way "
           "between these authors\n"
           "\t-h                       show this help\n\n\n"
           "Examples:\n"
           "./bin/program -i database/dblp.xml -o database/dblp.bin -a "
           "\"Quentin Bramas\"\n");
}

int parseArgs(int argc, char **argv, options_t *options) {
    options->inputFilename = NULL;
    options->outputFilename = NULL;
    options->inputFile = NULL;
    options->outputFile = NULL;
    options->action[ACTION_UNKNOWN] = 1;
    for (int a = 1; a < 6; a++)
        options->action[a] = NOT_TO_DO;
    options->authorNames[0] = NULL;
    options->authorNames[1] = NULL;

    int c;
    while ((c = getopt(argc, argv, "i:o:xrma:l:hp:N:")) != -1) {
        switch (c) {
        case 'i':
            options->inputFilename = optarg;
            break;
        case 'o':
            options->outputFilename = optarg;
            break;
        case 'x':
            options->action[ACTION_PARSE] = TO_DO;
            break;
        case 'r':
            options->action[ACTION_READ] = TO_DO;
            break;
        case 'm':
            options->action[ACTION_MAT] = TO_DO;
            break;
        case 'a':
            options->action[ACTION_MAT] = TO_DO;
            options->action[ACTION_SHOW_ARTICLES] = TO_DO;
            options->authorNames[0] = optarg;
            break;
        case 'N':
            options->N = atoi(optarg);
            break;
        case 'l':
            options->action[ACTION_MAT] = TO_DO;
            options->action[ACTION_SHOW_AUTHORS] = TO_DO;
            options->authorNames[0] = optarg;
            break;
        case 'p':
            if (options->authorNames[0] == NULL)
                options->authorNames[0] = optarg;
            else
                options->authorNames[1] = optarg;
            if (options->authorNames[0] == NULL ||
                options->authorNames[1] == NULL) {
                fprintf(stderr,
                        "One or two authors missing ! Usage : "
                        "./bin/program -i ... -o ... -p AUTHOR1 -p AUTHOR2\n");
                return ERROR_ARGS_PARSE;
            }
            break;
        case 'h':
            printUsage();
            return ERROR_ARGS_PARSE;
        case '?':
            if (optopt == 'i' || optopt == 'o' || optopt == 'a' ||
                optopt == 'l' || optopt == 'p' || optopt == 'N')
                fprintf(stderr, "Option -%c requires an argument.\n", optopt);
            else if (isprint(optopt))
                fprintf(stderr, "Unknown option `-%c'.\n", optopt);
            else
                fprintf(stderr, "Unknown option character `\\x%x'.\n", optopt);
            return ERROR_ARGS_PARSE;
        default:
            return ERROR_ARGS_PARSE;
        }
    }
    for (int k = 0; k < 6; k++) {
        if (options->action[k] != NOT_TO_DO) {
            options->action[ACTION_UNKNOWN] = 0;
        }
    }
    return OK;
}