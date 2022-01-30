#include <ctype.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "argsParser.h"
#include "io-utils.h"

void printUsage(void) {
    printf("\nUsage: ./bin/program OPTIONS\n\n"
           "Available options : ([ ] are optional)\n\n"
           "\t-i FICHIER                 indicate the xml to use\n"
           "\t-o FICHIER                 indicate the binary to use\n"
           "\t-x                         generate the binary file from the xml database\n"
           "\t-r                         read and print the binary structure for each document\n"
           "\t-g [-s]                    make the adjacence list from the binary file [and show it]\n"
           "\t-l PATTERN                 show all the authors containing PATTERN in their name\n"
           "\t-a AUTHOR [-y YEAR]        show the titles of the documents where the author has participated [in the year given]\n"
           "\t-a AUTHOR -n N             show all the authors at the distance N of the author given\n"
           "\t-a AUTHOR -d               show all the authors' distances in the connected component of the author given\n"
           "\t-p AUTHOR1 -p AUTHOR2      show the shortest path between these authors\n"
           "\t-p AUTHOR1 -p AUTHOR2 -d   show all the distances between these authors\n"
           "\t-c                         count the number of connected components\n"
           "\t-h                         show this help\n\n\n"
           "Example :\n"
           "./bin/program -i database/dblp.xml -o database/dblp.bin -x -a \"Quentin Bramas\" -y 2021\n");
}

error_t parseArgs(int argc, char **argv, options_t *options) {
    initOptions(options);
    int countAuthorsArguments = 0;
    int c;
    /* Turn to 1 the corresponding cells when actions are TO_DO */
    while ((c = getopt(argc, argv, "i:o:xrgsa:y:l:hp:dn:c")) != -1) {
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
        case 'g':
            options->action[ACTION_GRAPH] = TO_DO;
            break;
        case 's':
            options->action[ACTION_GRAPH] = TO_DO;
            options->action[ACTION_PRINT_GRAPH] = TO_DO;
            break;
        case 'a':
            options->action[ACTION_GRAPH] = TO_DO;
            options->action[ACTION_SHOW_ARTICLES] = TO_DO;
            options->authorNames[0] = optarg;
            break;
        case 'y':
            options->action[ACTION_SHOW_ARTICLES] = NOT_TO_DO;
            options->action[ACTION_SHOW_ARTICLES_YEAR] = TO_DO;
            options->year = atoi(optarg);
            break;
        case 'n':
            options->action[ACTION_GRAPH] = TO_DO;
            options->action[ACTION_SHOW_ARTICLES] = NOT_TO_DO;
            options->action[ACTION_DIJKSTRA] = TO_DO;
            options->action[ACTION_NEIGHBOURS] = TO_DO;
            options->N = atoi(optarg);
            break;
        case 'l':
            options->action[ACTION_GRAPH] = TO_DO;
            options->action[ACTION_SHOW_AUTHORS] = TO_DO;
            options->authorNames[0] = optarg;
            break;
        case 'p':
            if (options->authorNames[0] == NULL)
                options->authorNames[0] = optarg;
            else
                options->authorNames[1] = optarg;
            countAuthorsArguments++;
            options->action[ACTION_GRAPH] = TO_DO;
            options->action[ACTION_SHORTEST_PATH] = TO_DO;
            break;
        case 'd':
            options->action[ACTION_SHOW_ARTICLES] = NOT_TO_DO;
            options->action[ACTION_SHORTEST_PATH] = NOT_TO_DO;
            options->action[ACTION_DIJKSTRA] = TO_DO;
            options->action[ACTION_DISTANCE] = TO_DO;
            break;
        case 'c':
            options->action[ACTION_GRAPH] = TO_DO;
            options->action[ACTION_CONNECTED] = TO_DO;
            break;
        case 'h':
            printUsage();
            return HELP;
        case '?':
            if (optopt == 'i' || optopt == 'o' || optopt == 'a' || optopt == 'l' || optopt == 'p' || optopt == 'n' || optopt == 'y')
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
    /* Verify that the command entered is correct */
    if ((countAuthorsArguments == 2 && (options->authorNames[0] == NULL || options->authorNames[1] == NULL)) || countAuthorsArguments == 1) {
        fprintf(stderr, "One or two authors missing ! Usage : "
                        "./bin/program -i ... -o ... -p AUTHOR1 -p AUTHOR2\n");
        return ERROR_ARGS_PARSE;
    }
    for (int k = 1; k <= ACTIONS_NB; k++) {
        if (options->action[k] != NOT_TO_DO) {
            options->action[ACTION_UNKNOWN] = 0;
        }
    }
    return OK;
}