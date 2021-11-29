# include <stdio.h>
# include "io-utils.h"
# include <stdlib.h>

int openFiles(options_t * options, char * openMode){
    options->inputFile=fopen(options->inputFilename, "r");
    if (options->inputFile==NULL)
        abort();
    options->outputFile=fopen(options->outputFilename, openMode);
    if (options->outputFile==NULL)
        abort();
    return 0;
}

int closeFiles(options_t * options){
    fclose(options->inputFile);
    fclose(options->outputFile);
    return 0;
}