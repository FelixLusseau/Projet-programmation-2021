#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "program.h"
#include "argsParser.h"
#include "baseParser.h"
#include "io-utils.h"
#include "fonctionsMatrice.h"
#include "readBinary.h"

#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))

int interruptFlag = 0;

unsigned hash(unsigned char *str)
{
    unsigned hash = 5381;
    int c;

    while ((c = *str++))
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    //printf("%lu\n", hash);
    return hash;
}

unsigned int showALlAuthors(options_t * options){
    initSigaction();
    int16_t precAuthornb=0;
    structureBase_t structureBase;
    unsigned int max=0;
    while (1){
        initStructure(&structureBase, precAuthornb);
        structureBase = readEntryBin(options, -1);
        //printStruct(&structureBase);
        if (structureBase.authornb==0)
            break;
        for (int k=0; k<structureBase.authornb; k++){
                printf(" - %s : %u\n", structureBase.author[k], hash((unsigned char *)structureBase.author[k]));
                //printf("%lu\n", hash((unsigned char *)structureBase.author[k]));
                max=MAX(max, hash((unsigned char *)structureBase.author[k]));
        }
        if (interruptFlag==1)
            break;
        precAuthornb=structureBase.authornb;
    }
    return max;
}

int main()
{
    options_t options;
    options.inputFilename=NULL;
    options.outputFilename=NULL;
    options.inputFile=NULL;
    options.outputFile=fopen("../database/dblp.bin", "r");
    options.action=ACTION_UNKNOWN;
    options.authorNames[0]=NULL;
    options.authorNames[1]=NULL;

    showALlAuthors(&options);

    //printf("max : %u\n", showALlAuthors(&options));
    //printf("sizeof lu : %zu\n", sizeof(unsigned long int));
    return 0;
}

/* void testCreateListeAdj(void){
    char c0[]="author0";

    node *node0=CreateListAdj(c0);

    char *c1="Zb";
    char *c2="bc";
    char *c3="cd";
    char *c4="de";
    char *c5="ef";
    char *c6="ab";
    node *end=node0;
    end=appendNode(c1,end);
    end=appendNode(c2,end);
    end=appendNode(c3,end);
    end=appendNode(c4,end);
    end=appendNode(c5,end);
    end=appendNode(c6,end);

    appendEdge(5,1,node0);
    appendEdge(0,3,node0);
    appendEdge(3,2,node0);
    appendEdge(6,5,node0);
    appendEdge(6,3,node0);
    appendEdge(6,2,node0);

    printListNode(node0);
    printListEdge(node0);
    
    char author[]="ab";
    int test= AuthorInList(author,node0);
    printf("test présence author: %i\n",test);

    char author[] = "de";
    int test = AuthorInList(author, node0);
    printf("test présence author: %i\n", test);

    char author2[] = "z";
    int test2 = AuthorInList(author2, node0);
    printf("test présence author: %i\n", test2);

    freeListAdj(node0);
}



 */