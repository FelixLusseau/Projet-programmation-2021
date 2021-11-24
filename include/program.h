# ifndef PROGRAM_H
# define PROGRAM_H

# include <stdio.h>

typedef struct structureBase_t{
    char author[10][50];
    char title[200];
    int year;
} structureBase_t;

typedef struct options_t{
    char *inputFilename;
    char *outputFilename;

    FILE *inputFile;
    FILE *outputFile;
} options_t;


# endif