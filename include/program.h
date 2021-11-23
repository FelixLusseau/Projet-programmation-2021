# ifndef PROGRAM_H
# define PROGRAM_H

# include <stdio.h>

typedef struct structureBase_t{
    char name[50];
    char article[100];
    int year;
} structureBase_t;

typedef struct options_t{
    char *inputFilename;
    char *outputFilename;

    FILE *inputFile;
    FILE *outputFile;
} options_t;







# endif