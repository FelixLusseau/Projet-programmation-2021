# include <stdio.h>
# include "io-utils.h"

int openFiles(options_t * options){
    options->inputFile=fopen(options->inputFilename, "r");
    if (options->inputFile==NULL)
        return 4;
    options->outputFile=fopen(options->outputFilename, "w");
    if (options->outputFile==NULL)
        return 5;
    fclose(options->outputFile);
    options->outputFile=fopen(options->outputFilename, "r+");
    if (options->outputFile==NULL)
        return 5;
    return 0;
}

int closeFiles(options_t * options){
    fclose(options->inputFile);
    fclose(options->outputFile);
    return 0;
}