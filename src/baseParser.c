#include <stdio.h>
#include "baseParser.h"
#include <stdlib.h>

int parseBase(options_t *options, structureBase_t *structureBase)
{
    char *line = malloc(1000);
    if (line == NULL)
        return 6;
    int authornb = 0;
    while (fgets(line, 1000, options->inputFile) != NULL)
    {
        //printf("%s\n", line);
        if (line[1] == 'a' && line[2] == 'u')
        {
            int i = 8;
            if (structureBase->author[authornb] != NULL)
            {
                authornb++;
            }
            while (line[i] != '<')
            {
                structureBase->author[authornb][i - 8] = line[i];
                i++;
            }
            structureBase->author[authornb][i - 8] = '\0';
        }
        if (line[1] == 't' && line[2] == 'i')
        {
            int i = 7;
            while (line[i] != '<')
            {
                structureBase->title[i - 8] = line[i];
                i++;
            }
        }
        if (line[1] == 'y' && line[2] == 'e')
        {
            int i = 6;
            while (line[i] != '<')
            {
                structureBase->title[i - 8] = line[i];
                i++;
            }
        }
        if (line[0] == '<' && line[1] == '/')
            authornb=0;
    }
    fwrite(structureBase, sizeof(structureBase_t), 1, options->outputFile);
    return 0;
}

int readBin(options_t * options){
    structureBase_t structureBase;
    /* while (fread(&structureBase, sizeof(structureBase_t), 1, options->inputFile)!=0){
        printf("author : %s\ntitle : %s\nyear : %i\n", structureBase.author[0], structureBase.title, structureBase.year);
    } */
    fread(&structureBase, sizeof(structureBase_t), 1, options->inputFile);
    printf("author : %s\ntitle : %s\nyear : %i\n", structureBase.author[0], structureBase.title, structureBase.year);

    return 0;
}