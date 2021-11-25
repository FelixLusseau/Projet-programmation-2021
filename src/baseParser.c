#include <stdio.h>
#include "baseParser.h"
#include <stdlib.h>
#include <string.h>

void initStructure(structureBase_t * structureBase, int authornb){
    for (int h=0; h<=authornb; h++)
                structureBase->author[h][0]='\0';
    structureBase->title[0]='\0';
    structureBase->year=0;
}

int extractAuthor(structureBase_t * structureBase, int authornb, char * line)
{
    int i = 8;
    if (structureBase->author[authornb][0] != '\0')
    {
        authornb++;
    }
    while (line[i] != '<')
    {
        structureBase->author[authornb][i - 8] = line[i];
        i++;
    }
    structureBase->author[authornb][i - 8] = '\0';
    authornb++;
    return authornb;
}

void extractYear(structureBase_t * structureBase, char * line)
{
    int i = 6;
    char yeartmp[5];
    while (line[i] != '<')
    {
        yeartmp[i - 6] = line[i];
        i++;
    }
    structureBase->year = atoi(yeartmp);
}

void extractTitle1(structureBase_t * structureBase, char * line, int * titleLenght, int * titleEndOfLine){
    int i = 7;
    while (*titleEndOfLine == 0)
    {
        if (line[i] == '\n')
        {
            *titleLenght = i;
            *titleEndOfLine = 2;
            break;
        }
        if (line[0] == '<' && line[i] == '<')
        {
            *titleEndOfLine = 1;
            break;
        }
        structureBase->title[i - 7] = line[i];
        i++;
    }
    structureBase->title[i - 7] = '\0';
}

void extractTitle2(structureBase_t * structureBase, char * line, int * titleLenght, int titleEndOfLine){
    int i = *titleLenght;
            titleEndOfLine=0;
            while (titleEndOfLine==0)
            {
                if (line[i-*titleLenght] == '<'){
                    titleEndOfLine=1;
                    break;
                }
                if (line[i-*titleLenght]=='\n'){
                    titleEndOfLine++;
                    break;
                }
                structureBase->title[i - 7] = line[i-*titleLenght];
                i++;
            }
            structureBase->title[i-7]='\0';
}

int parseBase(options_t *options)
{
    char *line = malloc(1000);
    if (line == NULL)
        return 6;
    int authornb = 0;
    structureBase_t structureBase;
    initStructure(&structureBase, authornb);
    int titleEndOfLine=0;
    int titleLenght=0;
    while (fgets(line, 1000, options->inputFile) != NULL)
    {
        printf("line : %s\n", line);
        if (line[0] == '<' && line[1] == 'a' && line[2] == 'u')
            authornb=extractAuthor(&structureBase, authornb, line);
        if (line[0] == '<' && line[1] == 't' && line[2] == 'i')
            extractTitle1(&structureBase, line, &titleLenght, &titleEndOfLine);
        if (titleEndOfLine>=2 && line[0] != '<')
            extractTitle2(&structureBase, line, &titleLenght, titleEndOfLine);
        if (line[0] == '<' && line[1] == 'y' && line[2] == 'e')
            extractYear(&structureBase, line);
        if(strstr(line, "</article>")!=NULL 
            || strstr(line, "</inproceedings>")!=NULL 
            || strstr(line, "</proceedings>")!=NULL 
            || strstr(line, "</book>")!=NULL 
            || strstr(line, "</incollection>")!=NULL 
            || strstr(line, "</phdthesis>")!=NULL
            || strstr(line, "</mastersthesis>")!=NULL 
            || strstr(line, "</www>")!=NULL){
                for (int d=authornb+1; d<9; d++){
                    structureBase.author[d][0]='\0';
                }
                if (authornb != 0){
                    fwrite(&structureBase, sizeof(structureBase_t), 1, options->outputFile);
                    //printf("write :\nauthor 0 : %s\nauthor 1 : %s\ntitle : %s\nyear : %i\n\n", structureBase.author[0], structureBase.author[1], structureBase.title, structureBase.year);
                }
                initStructure(&structureBase, authornb);
                authornb=0;
                titleEndOfLine=0;
                titleLenght=0;
            }
    }
    return 0;
}

int readBin(options_t * options){
    fseek(options->outputFile, 0, SEEK_SET);
    int trigger=1;
    while (1){
        structureBase_t structureBase;
        trigger=fread(&structureBase, sizeof(structureBase_t), 1, options->outputFile);
        if (trigger==0)
            break;
        printf("read :\nauthor 0 : %s\nauthor 1 : %s\ntitle : %s\nyear : %i\n\n", structureBase.author[0], structureBase.author[1], structureBase.title, structureBase.year);
    }
    return 0;
}