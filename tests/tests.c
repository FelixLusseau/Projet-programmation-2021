#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "argsParser.h"
#include "baseParser.h"
#include "fonctionsMatrice.h"
#include "io-utils.h"
#include "program.h"
#include "readFunctions.h"

#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))
#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))

int interruptFlag = 0;

/* unsigned int showALlAuthors(options_t * options){
    initSigaction();
    int16_t precAuthornb=0;
    structureBase_t structureBase;
    unsigned int max=0;
    unsigned int min=hash((unsigned char *)structureBase.author[0]);
    while (1){
        initStructure(&structureBase, precAuthornb);
        structureBase = readEntryBin(options, -1);
        //printStruct(&structureBase);
        if (structureBase.authornb==0)
            break;
        for (int k=0; k<structureBase.authornb; k++){
                //printf(" - %s : %u\n", structureBase.author[k], hash((unsigned
char *)structureBase.author[k]));
                //printf("%lu\n", hash((unsigned char
*)structureBase.author[k])); max=MAX(max, hash((unsigned char
*)structureBase.author[k])); min=MIN(min, hash((unsigned char
*)structureBase.author[k]));
        }
        if (interruptFlag==1)
            break;
        precAuthornb=structureBase.authornb;
    }
    printf("max : %u\n", max);
    printf("min : %u\n", min);

    return OK;
} */

void testCreateListeAdj(void) {
    char c0[] = "author0";

    node *node0 = CreateListAdj(c0);

    char *c1 = "1";
    char *c2 = "2";
    char *c3 = "3";
    char *c4 = "4";
    char *c5 = "5";
    node *end = node0;
    end = appendNode(c1, end);
    end = appendNode(c2, end);
    end = appendNode(c3, end);
    end = appendNode(c4, end);
    end = appendNode(c5, end);

    appendEdge(5, 0, node0);
    appendEdge(5, 4, node0);
    appendEdge(5, 1, node0);
    appendEdge(1, 4, node0);
    appendEdge(4, 3, node0);
    appendEdge(3, 2, node0);

    printListNode(node0);
    printListEdge(node0);

    char author[] = "3";
    int test = AuthorInList(author, node0);
    printf("test présence author: %i\n", test);

    char author2[] = "z";
    int test2 = AuthorInList(author2, node0);
    printf("test présence author: %i\n", test2);

    // Dijkstra(6,node0,taille);
    // printDistance(6,node0);

    freeListAdj(node0);
}
int main() {
    testCreateListeAdj();
    /* options_t options;
    options.inputFilename=NULL;
    options.outputFilename=NULL;
    options.inputFile=NULL;
    options.outputFile=fopen("../database/dblp.bin", "r");
    options.action=ACTION_UNKNOWN;
    options.authorNames[0]=NULL;
    options.authorNames[1]=NULL; */

    // showALlAuthors(&options);

    char *tab = malloc(50000000 * sizeof(unsigned int) * sizeof(char *));
    // 50000000*sizeof(unsigned int)*sizeof(char *)=1600000000
    if (tab == NULL) {
        printf("nok\n");
        return 6;
    }
    sleep(10);
    // printf("max : %u\n", showALlAuthors(&options));
    printf("sizeof lu : %zu\n", sizeof(unsigned int));
    printf("%lu\n", 50000000 * sizeof(unsigned int) * sizeof(char *));
    return 0;
}
