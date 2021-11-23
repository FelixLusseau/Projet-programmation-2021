# include <stdio.h>
# include "baseParser.h"
# include <stdlib.h>



int parseBase(options_t * options, structureBase_t * structureBase){
    char * line = malloc(1000);
    if (line==NULL)
        return 6;
    fgets(line, 1000, options->inputFile);
    if (line[1]=='a' && line[2]=='u'){
        int i = 8;
        int authornb=0;
        while (structureBase->author[authornb]!=NULL){
            authornb++;
        }
        while (line[i]!='<'){
            structureBase->author[authornb][i-8]=line[i];
            i++;
        }
        structureBase->author[authornb][i-8]='\0';
    }
    if (line[1]=='t' && line[2]=='i'){
        int i = 7;
        while (line[i]!='<'){
            structureBase->title[i-8]=line[i];
            i++;
        }
    }
    if (line[1]=='y' && line[2]=='e'){
        int i = 6;
        while (line[i]!='<'){
            structureBase->title[i-8]=line[i];
            i++;
        }
    }
    fwrite(structureBase, sizeof(structureBase_t), 1, options->outputFile);
    return 0;

}