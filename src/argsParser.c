#include "argsParser.h"
#include <stdio.h>
#include <getopt.h>

void parseArgs(int argc, char ** argv, options_t * options){
    options->inputFilename=NULL;
    options->outputFilename=NULL;
    options->inputFile=NULL;
    options->outputFile=NULL;

    int c;
    while ((c = getopt (argc, argv, "i:o:")) != -1) 
    {
        switch (c)
        {
            case 'i':
                options->inputFilename = optarg;
                break;
            case 'o':
                options->outputFilename = optarg;
                break;
        }
    }

}