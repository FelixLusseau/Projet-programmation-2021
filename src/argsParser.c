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