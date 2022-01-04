#ifndef PROGRAM_H
#define PROGRAM_H

#include <signal.h>
#include <stdint.h>
#include <stdio.h>

#define ACTIONS_NB 10
#define HT_SIZE 50000000

typedef enum action_t {
    ACTION_UNKNOWN,
    ACTION_PARSE,
    ACTION_GRAPH,
    ACTION_READ,
    ACTION_SHOW_AUTHORS,
    ACTION_SHOW_ARTICLES,
    ACTION_DIJKSTRA,
    ACTION_SHORTEST_PATH,
    ACTION_DISTANCE,
    ACTION_NEIGHBOURS,
    ACTION_CONNECTED
} action_t;

typedef struct structureBase_t {
    int16_t year;
    int16_t titleLength;
    char title[2000];
    int16_t authornb;
    int8_t authorlengths[500];
    char author[500][100];
} structureBase_t;

typedef struct options_t {
    char *inputFilename;
    char *outputFilename;
    int action[ACTIONS_NB + 1];
    char *authorNames[2];
    int N;

    FILE *inputFile;
    FILE *outputFile;
} options_t;

typedef enum action_status_t {
    NOT_TO_DO,
    TO_DO,
} action_status_t;

typedef enum error_t {
    OK,
    ERROR_ARGS_PARSE,
    ERROR_MISSING_ACTION,
    ERROR_OPEN_DATABASE,
    ERROR_OPEN_BIN,
    ERROR_XML,
    ERROR_BIN,
    ERROR_BASE_PARSE,
    ERROR_READ,
    ERROR_MAT,
    ERROR_SHOW_ARTICLES,
    ERROR_SHOW_AUTHORS,
    ERROR_NO_AUTHOR,
    ERROR_GRAPH,
    ERROR_NODE_EQ_NULL,
    ERROR_PATH
} error_t;

#endif