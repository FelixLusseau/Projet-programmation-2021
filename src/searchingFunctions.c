#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/signal.h>
#include <errno.h>

#include "baseParser.h"
#include "io-utils.h"
#include "readBinary.h"
#include "searchingFunctions.h"

extern int interruptFlag;

int showArticles(options_t * options){
    initSigaction();
    int16_t precAuthornb=0;
    int authorWritten = 0;
    structureBase_t structureBase;
    printf("Articles of ");
    while (1){
        initStructure(&structureBase, precAuthornb);
        structureBase = readEntryBin(options, -1);
        //printStruct(&structureBase);
        if (structureBase.authornb==0 || interruptFlag==1)
            break;
        for (int k=0; k<structureBase.authornb; k++){
            if (strstr(structureBase.author[k], options->authorNames[0])){
                if (authorWritten==0){
                    printf("%s : \n", structureBase.author[k]);
                    authorWritten=1;
                }
                printf(" - %s\n", structureBase.title);
            }
        }
        precAuthornb=structureBase.authornb;
    }
    return OK;
}

int showAuthors(options_t * options){
    initSigaction();
    int16_t precAuthornb=0;
    structureBase_t structureBase;
    printf("Authors containing %s in their name : \n", options->authorNames[0]);
    while (1){
        initStructure(&structureBase, precAuthornb);
        structureBase = readEntryBin(options, -1);
        //printStruct(&structureBase);
        if (structureBase.authornb==0)
            break;
        for (int k=0; k<structureBase.authornb; k++){
            if (strstr(structureBase.author[k], options->authorNames[0])){
                printf(" - %s\n", structureBase.author[k]);
            }
        }
        if (interruptFlag==1)
            break;
        precAuthornb=structureBase.authornb;
    }
    return OK;
}