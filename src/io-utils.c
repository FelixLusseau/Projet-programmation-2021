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
            return ERROR_OPEN_FILES;
        }
    options->outputFile=fopen(options->outputFilename, openMode);
    if (options->outputFile==NULL)
        {
            fprintf(stderr, "%s : %s.\n", strerror(errno), options->outputFilename);
            return ERROR_OPEN_FILES;
            }
    return OK;
}

int closeFiles(options_t * options){
    fclose(options->inputFile);
    fclose(options->outputFile);
    return 0;
}

const char * errorToString(error_t err) 
{
    switch(err) {
        case ERROR_ARGS_PARSE: return "Error while parsing arguments";
        case ERROR_OPEN_FILES: return "Error while opening files";
        case ERROR_BASE_PARSE: return "Error while parsing base";
        case ERROR_READ: return "Error while reading the binary";
        case ERROR_MAT: return "Error while generating the matrice";
        case ERROR_SHOW_ARTICLES: return "Error while showing the articles";
        case ERROR_SHOW_AUTHORS: return "Error while showing the authors";
        case ERROR_GRAPH: return "Error while generating the graph";
        default:
        case OK: return "";
    }
}