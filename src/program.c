#include "program.h"
#include "argsParser.h"
#include "baseParser.h"
#include "io-utils.h"
#include "fonctionsMatrice.h"

int interruptFlag = 0;

 void testCreateListeAdj(void){
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

    char author2[]="z";
    int test2= AuthorInList(author2,node0);
    printf("test présence author: %i\n",test2);

    freeListAdj(node0);
}

int main(int argc, char ** argv){
    initSigaction();
    int exitCode;
    options_t options;
    exitCode=parseArgs(argc, argv, &options);
    node *node0;
    if(exitCode) goto error;
    switch (options.action)
    {
    case ACTION_PARSE:
        exitCode=openFiles(&options, "w");
        if(exitCode) goto error;
        exitCode=parseBase(&options);
        if(exitCode) goto error;
        printf("Database parsing ok ! \n");
        /* char * line = malloc(1000);
        int i=0;
        while (fgets(line, 1000, options.inputFile) != NULL){
            i++;
        } */
         break;
    case ACTION_READ:
        exitCode=openFiles(&options, "r");
        if(exitCode) goto error;
        exitCode=readEntireBin(&options);
        if(exitCode) goto error;
        //structureBase_t structureBase;
        //initStructure(&structureBase, 0);
        //structureBase = readEntryBin(&options, 7);
        //printStruct(&structureBase);
        break;
    case ACTION_MAT:
        exitCode=openFiles(&options, "r");
        node0=DoListAdjDeBin(&options);
        printListNode(node0);
        printListEdge(node0);
        //testCreateListeAdj();
        freeListAdj(node0);
        break;
    case ACTION_SHOW_ARTICLES:
        exitCode=openFiles(&options, "r");
        if(exitCode) goto error;
        exitCode=showArticles(&options);
        if(exitCode) goto error;
        break;
    case ACTION_SHOW_AUTHORS:
        exitCode=openFiles(&options, "r");
        if(exitCode) goto error;
        exitCode=showAuthors(&options);
        if(exitCode) goto error;
        break;
    default:
        fprintf(stderr, "Action is missing\n");
        goto error;
        break;
    }
    closeFiles(&options);

    error: if(exitCode){ 
        fprintf(stderr, "%s\n", errorToString(exitCode));
        return exitCode;
    }
    return OK; 
}





/* int main(int argc, char ** argv){
    options_t options;
    parseArgs(argc, argv, &options);
    openFiles(&options);
    parseBase(&options);
    structureBase_t structureBase;
    initStructure(&structureBase, 0);
    structureBase = readEntryBin(&options, 0);
    node *node0 =DoListAdjDeBin(&options);
    printListNode(node0);
    printListEdge(node0);
    freeListAdj(node0);
    closeFiles(&options);
    return 0;
}    */
