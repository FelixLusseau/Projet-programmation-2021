# ifndef PROGRAM_H
# define PROGRAM_H

# include <stdio.h>
# include <signal.h>

typedef enum action_t {
    ACTION_UNKNOWN,
    ACTION_PARSE,
    ACTION_READ,
    ACTION_MAT,
    ACTION_SHOW_ARTICLES
} action_t;

typedef struct structureBase_t{
    int endOfFileFlag;
    int year;
    int titleLength;
    char title[2000];
    int authornb;
    int authorlengths[500];
    char author[500][50];
} structureBase_t;

typedef struct options_t{
    action_t action;
    char *inputFilename;
    char *outputFilename;

    FILE *inputFile;
    FILE *outputFile;
} options_t;


# endif