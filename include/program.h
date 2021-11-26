# ifndef PROGRAM_H
# define PROGRAM_H

# include <stdio.h>
# include <signal.h>

typedef struct structureBase_t{
    int endOfFileFlag;
    char title[1500];
    int year;
    char author[500][50];
} structureBase_t;

typedef struct options_t{
    char *inputFilename;
    char *outputFilename;

    FILE *inputFile;
    FILE *outputFile;
} options_t;


# endif