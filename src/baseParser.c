#include <stdio.h>
#include "baseParser.h"
#include <stdlib.h>
#include <string.h>
#include <sys/signal.h>
#include <errno.h>

extern int interruptFlag;

void handleSignal(){
    int carac;
    fprintf(stderr, "\nCTRL+C pressed !\nDo you want to save and exit ? [Y=1/N=0]\nRép : ");
    scanf("%i", &carac);
    if (carac==1){
        interruptFlag=1;
        fprintf(stderr, "Exit !\n");
    }
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
    structureBase->year=0;
    structureBase->titleLength=0;
    structureBase->title[0]='\0';
    structureBase->authornb=0;
    for (int h=0; h<=authornb; h++){
                structureBase->author[h][0]='\0';
                structureBase->authorlengths[h]=0;
    }
}

void extractAuthor(structureBase_t * structureBase, int * authornb, char * line)
{
    int dec=8;
    int i = dec;
    if (line[i-1]!='>'){
    while (line[i]!='>'){
        i++;
    }
    i++;
    dec=i;
    }
    while (line[i] != '<')
    {
        structureBase->author[*authornb][i - dec] = line[i];
        i++;
    }
    structureBase->author[*authornb][i - dec] = '\0';
    structureBase->authorlengths[*authornb]=i-dec;
    *authornb+=1;
    structureBase->authornb=*authornb;
    //printf("authornb : %i, authorlength : %i\n, author : %s\n", structureBase->authornb, structureBase->authorlengths[0], structureBase->author[*authornb-1]);
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

void extractTitle2(structureBase_t *structureBase, char *line, int *titleLength, int titleEndOfLine)
{
    int i = *titleLength;
    titleEndOfLine = 0;
    while (titleEndOfLine == 0)
    {
        if (line[i - *titleLength] == '<')
        {
            titleEndOfLine = 1;
            *titleLength = i;
            structureBase->titleLength = i;
            break;
        }
        if (line[i - *titleLength] == '\n')
        {
            *titleLength = i;
            structureBase->titleLength = i;
            titleEndOfLine++;
            break;
        }
        structureBase->title[i] = line[i - *titleLength];
        i++;
    }
    *titleLength = i;
    structureBase->titleLength = i;
    structureBase->title[i] = '\0';
}

int parseBase(options_t *options)
{
    initSigaction();
    errno = 0;
    unsigned long long int linenb=0;
    char *line = malloc(1000);
    if (line == NULL){
        fprintf(stderr, "%s.\n", strerror(errno));
        return 6;
    }
    int authornb = 0;
    structureBase_t structureBase;
    initStructure(&structureBase, authornb);
    int titleEndOfLine=0;
    int titleLength=0;
    while (fgets(line, 1000, options->inputFile) != NULL)
    {
        linenb++;
        //printf("line %lli : %s\n", linenb, line);
        if (line[0] == '<' && line[1] == 'a' && line[2] == 'u')
            extractAuthor(&structureBase, &authornb, line);
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
                if (authornb != 0){
                    fwrite(&structureBase, 2*sizeof(int16_t)+titleLength+1, 1, options->outputFile);
                    fwrite(&structureBase.authornb, sizeof(int16_t), 1, options->outputFile);
                    fwrite(&structureBase.authorlengths, authornb*sizeof(int8_t), 1, options->outputFile);
                    for (int m=0; m<structureBase.authornb; m++){
                        fwrite(structureBase.author[m], structureBase.authorlengths[m]+1, 1, options->outputFile);
                    }
                    //printf("write :\nauthor 0 : %s\nauthor 1 : %s\ntitle : %s\nyear : %i\n\n", structureBase.author[0], structureBase.author[1], structureBase.title, structureBase.year);
                }
                if (interruptFlag==1){
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
    initSigaction();
    fseek(options->outputFile, 0, SEEK_SET);
    int trigger=1;
    int16_t precAuthornb=0;
    while (1){
        structureBase_t structureBase;
        initStructure(&structureBase, precAuthornb);
        trigger=fread(&structureBase, 2*sizeof(int16_t), 1, options->outputFile);
        //printf("length : %i\n", structureBase.titleLength);
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
        printf("read :\ntitle : %s\nyear : %i\n", structureBase.title, structureBase.year);
        for (int r=0; r<structureBase.authornb; r++){
            printf("author %i : %s\n", r, structureBase.author[r]);
        }
        printf("\n");
        //printf("read :\ntitle : %s\nyear : %i\n\n", structureBase.title, structureBase.year);

    }
    return 0;
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

void showArticles(options_t * options){
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
}

void showAuthors(options_t * options){
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
}