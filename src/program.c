#include "program.h"
#include "argsParser.h"
#include "baseParser.h"
#include "io-utils.h"
#include "fonctionsMatrice.h"

int main(int argc, char ** argv){
    options_t options;
    parseArgs(argc, argv, &options);
    openFiles(&options);
    parseBase(&options);
    //readEntireBin(&options);
    structureBase_t structureBase;
    initStructure(&structureBase, 0);
    structureBase = readEntryBin(&options, 7);
    closeFiles(&options);
    return 0;
}

/* #include "fonctionsMatrice.h"

void testCreateListeAdj(void){
    char c0[]="author0";

    node *node0=CreateListAdj(c0);

    char c1[]="ab";
    char c2[]="bc";
    char c3[]="cd";
    char c4[]="de";
    char c5[]="ef";
    char c6[]="fg";

    appendNode(node0,c1);
    appendNode(node0,c2);
    appendNode(node0,c3);
    appendNode(node0,c4);
    appendNode(node0,c5);
    appendNode(node0,c6);
    printListNode(node0);
    printListEdge(node0);
    
    appendEdge(5,1,node0);
    appendEdge(0,3,node0);
    appendEdge(3,2,node0);
    appendEdge(6,5,node0);
    appendEdge(6,3,node0);
    appendEdge(6,2,node0);
    printListNode(node0);
    printListEdge(node0);
    //printListAdj(node0);
    char author[]="de";
    int test= AuthorInList(author,node0);
    printf("test présence author: %i\n",test);

    freeListAdj(node0);
}

int main(void){
    testCreateListeAdj();
    return 0;
} */

/* int main(int argc, char ** argv){
    options_t options;
    parseArgs(argc, argv, &options);
    openFiles(&options);
    node *node0 =DoListAdjDeBin(&options);
    printListNode(node0);
    printListEdge(node0);
    freeListAdj(node0);
    closeFiles(&options);
    return 0;
} */