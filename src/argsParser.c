#include "argsParser.h"
#include <stdio.h>
#include <getopt.h>

void argsParser(int argc, char ** argv, options_t options){
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
                options->inputFilename=optargs;
                break;
            case 'o':
                options->outputFilename=optargs;
                break;
        }
    }

}