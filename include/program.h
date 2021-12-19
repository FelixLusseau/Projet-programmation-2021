#ifndef PROGRAM_H
#define PROGRAM_H

#include <signal.h>
#include <stdint.h>
#include <stdio.h>

typedef enum action_t {
    ACTION_UNKNOWN,
    ACTION_PARSE,
    ACTION_MAT,
    ACTION_READ,
    ACTION_SHOW_ARTICLES,
    ACTION_SHOW_AUTHORS
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
    int action[6];
    char *authorNames[2];
    int N;

    FILE *inputFile;
    FILE *outputFile;
} options_t;

typedef enum action_status_t {
    NOT_TO_DO,
    TO_DO,
    DONE_SUCCESSFULLY
} action_status_t;

typedef enum error_t {
    OK,
    ERROR_ARGS_PARSE,
    ERROR_MISSING_ACTION,
    ERROR_OPEN_DATABASE,
    ERROR_OPEN_BIN,
    ERROR_XML,
    ERROR_BASE_PARSE,
    ERROR_READ,
    ERROR_MAT,
    ERROR_SHOW_ARTICLES,
    ERROR_SHOW_AUTHORS,
    ERROR_GRAPH
} error_t;

#endif