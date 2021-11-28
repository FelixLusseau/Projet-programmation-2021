#include <stdio.h>
#include "baseParser.h"
#include <stdlib.h>
#include <string.h>
#include <sys/signal.h>

int interruptFlag = 0;

void handleSignal(){
    int carac;
    fprintf(stderr, "\nCTRL+C pressed !\nDo you want to save and exit ? [Y=1/N=0]\nRép : ");
    scanf("%i", &carac);
    if (carac==1)
        interruptFlag=1;
    //printf("interruptFlag : %i\n", interruptFlag);
}
void initSigaction(){
    struct sigaction sa;
    sa.sa_handler = &handleSignal;
    sa.sa_flags = SA_RESTART;
    sigfillset(&sa.sa_mask);
    if (sigaction(SIGINT, &sa, NULL) == -1) {
        perror("Error: cannot handle SIGINT"); // Should not happen
    }
}


void initStructure(structureBase_t * structureBase, int authornb){
    /* for (int h=0; h<=authornb; h++)
                strcpy(structureBase->author[h], ""); */
    (void)authornb;
    //structureBase->author[0][0]='\0';
    structureBase->titleLength=0;
    structureBase->title[0]='\0';
    structureBase->year=0;
    structureBase->endOfFileFlag=1;
}

/* void extractAuthor(structureBase_t * structureBase, int * authornb, char * line)
{
    int i = 8;
    while (line[i] != '<')
    {
        structureBase->author[*authornb][i - 8] = line[i];
        i++;
    }
    structureBase->author[*authornb][i - 8] = '\0';
    *authornb+=1;
} */

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

void extractTitle1(structureBase_t * structureBase, char * line, int * titleLength, int * titleEndOfLine){
    int i = 7;
    while (*titleEndOfLine == 0)
    {
        if (line[i] == '\n')
        {
            *titleLength = i-7;
            structureBase->titleLength = i-7;
            *titleEndOfLine = 2;
            break;
        }
        if (line[0] == '<' && line[i] == '<')
        {
            *titleEndOfLine = 1;
            *titleLength = i-7;
            structureBase->titleLength = i-7;
            break;
        }
        structureBase->title[i - 7] = line[i];
        i++;
    }
    *titleLength = i-7;
    structureBase->titleLength = i-7;
    structureBase->title[i - 7] = '\0';
}

void extractTitle2(structureBase_t * structureBase, char * line, int * titleLength, int titleEndOfLine){
    int i = *titleLength;
            titleEndOfLine=0;
            while (titleEndOfLine==0)
            {
                if (line[i-*titleLength] == '<'){
                    titleEndOfLine=1;
                    *titleLength = i;
                    structureBase->titleLength = i;
                    break;
                }
                if (line[i-*titleLength]=='\n'){
                    *titleLength = i;
                    structureBase->titleLength = i;
                    titleEndOfLine++;
                    break;
                }
                structureBase->title[i] = line[i-*titleLength];
                i++;
            }
            *titleLength = i;
            structureBase->titleLength = i;
            structureBase->title[i]='\0';
}

int parseBase(options_t *options)
{
    initSigaction();
    unsigned long long int linenb=0;
    char *line = malloc(1000);
    if (line == NULL)
        return 6;
    int authornb = 0;
    structureBase_t structureBase;
    initStructure(&structureBase, authornb);
    int titleEndOfLine=0;
    int titleLength=0;
    while (fgets(line, 1000, options->inputFile) != NULL)
    {
        linenb++;
        //printf("line %lli : %s\n", linenb, line);
        /* if (line[0] == '<' && line[1] == 'a' && line[2] == 'u')
            extractAuthor(&structureBase, &authornb, line); */
        if (line[0] == '<' && line[1] == 't' && line[2] == 'i')
            extractTitle1(&structureBase, line, &titleLength, &titleEndOfLine);
        if (titleEndOfLine>=2 && line[0] != '<')
            extractTitle2(&structureBase, line, &titleLength, titleEndOfLine);
        //printf("length : %i\n", titleLength);
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
                /* for (int d=authornb+1; d<499; d++){
                    structureBase.author[d][0]='\0';
                } */
                //printf("authornb : %i\n", authornb);
                if (authornb == 0){
                    fwrite(&structureBase, 3*sizeof(int)+titleLength+1, 1, options->outputFile);
                    //printf("write :\nauthor 0 : %s\nauthor 1 : %s\ntitle : %s\nyear : %i\n\n", structureBase.author[0], structureBase.author[1], structureBase.title, structureBase.year);
                }
                if (interruptFlag==1){
                    fprintf(stderr, "Exit !\n");
                    break;
                }
                initStructure(&structureBase, authornb);
                authornb=0;
                titleEndOfLine=0;
                titleLength=0;
            }
    }
    free(line);
    return 0;
}

int readEntireBin(options_t * options){
    fseek(options->outputFile, 0, SEEK_SET);
    int trigger=1;
    while (1){
        structureBase_t structureBase;
        trigger=fread(&structureBase, 3*sizeof(int), 1, options->outputFile);
        //printf("length : %i\n", structureBase.titleLength);
        trigger=fread(structureBase.title, structureBase.titleLength+1, 1, options->outputFile);
        if (trigger==0)
            break;
        if (interruptFlag==1)
            break;
        //printf("read :\nauthor 0 : %s\nauthor 1 : %s\ntitle : %s\nyear : %i\n\n", structureBase.author[0], structureBase.author[1], structureBase.title, structureBase.year);
        printf("read :\ntitle : %s\nyear : %i\n\n", structureBase.title, structureBase.year);

    }
    return 0;
}

structureBase_t readEntryBin(options_t * options, int curseur){
    structureBase_t structureBase;
    int trigger=1;
    fseek(options->outputFile, curseur*sizeof(structureBase_t), SEEK_SET);
    trigger=fread(&structureBase, sizeof(structureBase_t), 1, options->outputFile);
    if (trigger==0)
        structureBase.endOfFileFlag=0;
    //printf("read :\nauthor 0 : %s\nauthor 1 : %s\ntitle : %s\nyear : %i\n\n", structureBase.author[0], structureBase.author[1], structureBase.title, structureBase.year);
    return structureBase;
}

void readLinesOnly(options_t *options){
    initSigaction();
    unsigned long long int linenb=0;
    char *line = malloc(1000);
    if (line == NULL)
        return ;
    while (fgets(line, 1000, options->inputFile) != NULL)
    {
        linenb++;
        //printf("line %lli : %s\n", linenb, line);
        if (interruptFlag==1){
            fprintf(stderr, "Exit !\n");
            break;
        }
    }
}