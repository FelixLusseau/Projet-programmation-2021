#include "argsParser.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>

void parseArgs(int argc, char ** argv, options_t * options){
    options->inputFilename=NULL;
    options->outputFilename=NULL;
    options->inputFile=NULL;
    options->outputFile=NULL;
    options->action=ACTION_UNKNOWN;

    int c;
    while ((c = getopt (argc, argv, "i:o:xrma:")) != -1) 
    {
        switch (c)
        {
            case 'i':
                options->inputFilename = optarg;
                break;
            case 'o':
                options->outputFilename = optarg;
                break;
            case 'x':
                options->action=ACTION_PARSE;
                break;
            case 'r':
                options->action=ACTION_READ;
                break;
            case 'm':
                options->action=ACTION_MAT;
                break;
            case 'a':
                options->action=ACTION_SHOW_ARTICLES;
                break;
            case '?':
                if (optopt == 'i' 
                    || optopt == 'o' )
                    fprintf (stderr, "Option -%c requires an argument.\n", optopt);
                else if (isprint (optopt))
                    fprintf (stderr, "Unknown option `-%c'.\n", optopt);
                else
                    fprintf (stderr,
                            "Unknown option character `\\x%x'.\n",
                            optopt);
                abort();
            default:
                abort ();
        }
    }

}