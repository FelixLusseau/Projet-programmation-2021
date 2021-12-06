#include <stdio.h>
#include "baseParser.h"
#include <stdlib.h>
#include <string.h>
#include <sys/signal.h>
#include <errno.h>
#include "io-utils.h"

extern int interruptFlag;

void initStructure(structureBase_t *structureBase, int authornb)
{
    structureBase->year = 0;
    structureBase->titleLength = 0;
    structureBase->title[0] = '\0';
    structureBase->authornb = 0;
    for (int h = 0; h <= authornb; h++)
    {
        structureBase->author[h][0] = '\0';
        structureBase->authorlengths[h] = 0;
    }
}

void extractAuthor(structureBase_t * structureBase, char * line)
{
    int dec = 8;
    int i = dec;
    if (line[i - 1] != '>')
    {
        while (line[i] != '>')
        {
            i++;
        }
        i++;
        dec = i;
    }
    while (line[i] != '<')
    {
        structureBase->author[structureBase->authornb][i - dec] = line[i];
        i++;
    }
    structureBase->author[structureBase->authornb][i - dec] = '\0';
    structureBase->authorlengths[structureBase->authornb] = i - dec;
    structureBase->authornb += 1;
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
    yeartmp[i - 6] = '\0';
    structureBase->year = atoi(yeartmp);
}

void extractTitle1(structureBase_t * structureBase, char * line, int * titleEndOfLine){
    int i = 7;
    while (*titleEndOfLine == 0)
    {
        if (line[i] == '\n')
        {
            structureBase->titleLength = i-7;
            *titleEndOfLine = 2;
            break;
        }
        if (line[0] == '<' && line[i] == '<')
        {
            *titleEndOfLine = 1;
            structureBase->titleLength = i-7;
            break;
        }
        structureBase->title[i - 7] = line[i];
        i++;
    }
    structureBase->titleLength = i-7;
    structureBase->title[i - 7] = '\0';
}

void extractTitle2(structureBase_t *structureBase, char *line, int titleEndOfLine)
{
    int i = structureBase->titleLength;
    titleEndOfLine = 0;
    while (titleEndOfLine == 0)
    {
        if (line[i - structureBase->titleLength] == '<' || line[i - structureBase->titleLength] == '\n')
        {
            structureBase->titleLength = i;
            titleEndOfLine++;
            break;
        }
        structureBase->title[i] = line[i - structureBase->titleLength];
        i++;
    }
    structureBase->titleLength = i;
    structureBase->title[i] = '\0';
}

int parseBase(options_t *options)
{
    initSigaction();
    errno = 0;
    unsigned long long int linenb=0;
    char *line = malloc(1500);
    if (line == NULL){
        fprintf(stderr, "%s.\n", strerror(errno));
        return ERROR_BASE_PARSE;
    }
    structureBase_t structureBase;
    initStructure(&structureBase, 0);
    int titleEndOfLine=0;
    while (fgets(line, 1500, options->inputFile) != NULL)
    {
        linenb++;
        //printf("line %lli : %s\n", linenb, line);
        if (line[0] == '<' && line[1] == 'a' && line[2] == 'u')
            extractAuthor(&structureBase, line);
        else if (line[0] == '<' && line[1] == 't' && line[2] == 'i')
            extractTitle1(&structureBase, line, &titleEndOfLine);
        else if (titleEndOfLine>=2 && line[0] != '<')
            extractTitle2(&structureBase, line, titleEndOfLine);
        else if (line[0] == '<' && line[1] == 'y' && line[2] == 'e')
            extractYear(&structureBase, line);
        //else if(strstr(line, "</article>")!=NULL 
        //    || strstr(line, "</inproceedings>")!=NULL 
        //    || strstr(line, "</proceedings>")!=NULL 
        //    || strstr(line, "</book>")!=NULL 
        //    || strstr(line, "</incollection>")!=NULL 
        //    || strstr(line, "</phdthesis>")!=NULL
        //    || strstr(line, "</mastersthesis>")!=NULL 
        //    || strstr(line, "</www>")!=NULL){

        else if(line[0] == '<' && line[1] == '/'){
                if (structureBase.authornb != 0){
                    fwrite(&structureBase, 2*sizeof(int16_t)+structureBase.titleLength+1, 1, options->outputFile);
                    fwrite(&structureBase.authornb, sizeof(int16_t), 1, options->outputFile);
                    fwrite(&structureBase.authorlengths, structureBase.authornb*sizeof(int8_t), 1, options->outputFile);
                    for (int m=0; m<structureBase.authornb; m++){
                        fwrite(structureBase.author[m], structureBase.authorlengths[m]+1, 1, options->outputFile);
                    }
                    //printf("write :\nauthor 0 : %s\nauthor 1 : %s\ntitle : %s\nyear : %i\n\n", structureBase.author[0], structureBase.author[1], structureBase.title, structureBase.year);
                }
                if (interruptFlag==1){
                    break;
                }
                initStructure(&structureBase, structureBase.authornb);
                titleEndOfLine=0;
            }
    }
    free(line);
    return OK;
}

int readEntireBin(options_t * options){
    initSigaction();
    fseek(options->outputFile, 0, SEEK_SET);
    int trigger=1;
    int16_t precAuthornb=0;
    while (1){
        structureBase_t structureBase;
        initStructure(&structureBase, precAuthornb);
        trigger=fread(&structureBase, 2*sizeof(int16_t), 1, options->outputFile);
        trigger=fread(&structureBase.title, structureBase.titleLength+1, 1, options->outputFile);
        trigger=fread(&structureBase.authornb, sizeof(int16_t), 1 ,options->outputFile);
        trigger=fread(structureBase.authorlengths, structureBase.authornb*sizeof(int8_t), 1, options->outputFile);
        for (int m=0; m<structureBase.authornb; m++){
            trigger=fread(structureBase.author[m], structureBase.authorlengths[m]+1, 1, options->outputFile);
        }
        if (trigger==0)
            break;
        if (interruptFlag==1)
            break;
        precAuthornb=structureBase.authornb;
        printf("title : %s\nyear : %i\n", structureBase.title, structureBase.year);
        for (int r=0; r<structureBase.authornb; r++){
            printf("author %i : %s\n", r, structureBase.author[r]);
        }
        printf("\n");
        //printf("read :\ntitle : %s\nyear : %i\n\n", structureBase.title, structureBase.year);
    }
    return OK;
}

structureBase_t readEntryBin(options_t *options, int curseur)
{
    fseek(options->outputFile, 0, SEEK_SET);
    int16_t precAuthornb=0;
    int count = 0;
    structureBase_t structureBase;
    while (count <= curseur)
    {
        initStructure(&structureBase, precAuthornb);
        fread(&structureBase, 2 * sizeof(int16_t), 1, options->outputFile);
        fread(&structureBase.title, structureBase.titleLength + 1, 1, options->outputFile);
        fread(&structureBase.authornb, sizeof(int16_t), 1, options->outputFile);
        fread(structureBase.authorlengths, structureBase.authornb * sizeof(int8_t), 1, options->outputFile);
        for (int m = 0; m < structureBase.authornb; m++)
        {
            fread(structureBase.author[m], structureBase.authorlengths[m] + 1, 1, options->outputFile);
        }
        count++;
        precAuthornb=structureBase.authornb;
    }
    return structureBase;
}

void printStruct(structureBase_t * structureBase){
    printf("read :\ntitle : %s\nyear : %i\n", structureBase->title, structureBase->year);
    for (int r=0; r<structureBase->authornb; r++){
        printf("author %i : %s\n", r, structureBase->author[r]);
    }
    printf("\n");
}

int showArticles(options_t * options){
    initSigaction();
    int curseur=1;
    int16_t precAuthornb=0;
    int authorWritten = 0;
    printf("Articles of ");
    while (1){
        structureBase_t structureBase;
        initStructure(&structureBase, precAuthornb);
        structureBase = readEntryBin(options, curseur);
        //printStruct(&structureBase);
        if (structureBase.authornb==0)
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
        if (interruptFlag==1)
            break;
        precAuthornb=structureBase.authornb;
        curseur++;
    }
    return OK;
}

int showAuthors(options_t * options){
    initSigaction();
    int curseur=1;
    int16_t precAuthornb=0;
    printf("Authors containing %s in their name : \n", options->authorNames[0]);
    while (1){
        structureBase_t structureBase;
        initStructure(&structureBase, precAuthornb);
        structureBase = readEntryBin(options, curseur);
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
        curseur++;
    }
    return OK;
}