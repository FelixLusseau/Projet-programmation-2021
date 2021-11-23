# include <stdio.h>
# include "io-utils.h"

void openFile(options * options){
    options->inputFile=fopen(options->inputFilename, "r");
    if (options->inputFile==NULL)
        return 4;
    options->outputFile=fopen(options->outputFilename, "w");
    if (options->outputFile==NULL)
        return 5;
}
