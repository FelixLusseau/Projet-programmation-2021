# include <stdio.h>
# include "io-utils.h"
# include <stdlib.h>
# include <errno.h>
# include <string.h>


int openFiles(options_t * options, char * openMode){
    errno=0;
    options->inputFile=fopen(options->inputFilename, "r");
    if (options->inputFile==NULL)
        {
            fprintf(stderr, "%s : %s.\n", strerror(errno), options->inputFilename);
            abort();
        }
    options->outputFile=fopen(options->outputFilename, openMode);
    if (options->outputFile==NULL)
        {
            fprintf(stderr, "%s : %s.\n", strerror(errno), options->outputFilename);
            abort();
            }
    return 0;
}

int closeFiles(options_t * options){
    fclose(options->inputFile);
    fclose(options->outputFile);
    return 0;
}