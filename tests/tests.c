#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "program.h"
#include "argsParser.h"
#include "baseParser.h"
#include "io-utils.h"
#include "fonctionsMatrice.h"

int interruptFlag = 0;

void testCreateListeAdj(void){
    char c0[]="author0";

    node *node0=CreateListAdj(c0);

    char *c1="1";
    char *c2="2";
    char *c3="3";
    char *c4="4";
    char *c5="5";
    char *c6="6";
    node *end=node0;
    end=appendNode(c1,end);
    end=appendNode(c2,end);
    end=appendNode(c3,end);
    end=appendNode(c4,end);
    end=appendNode(c5,end);
    end=appendNode(c6,end);

    appendEdge(5,0,node0);
    appendEdge(5,4,node0);
    appendEdge(5,1,node0);
    appendEdge(1,4,node0);
    appendEdge(4,3,node0);
    appendEdge(3,2,node0);
    appendEdge(6,2,node0);

    printListNode(node0);
    printListEdge(node0);
    
    char author[]="3";
    int test= AuthorInList(author,node0);
    printf("test présence author: %i\n",test);

    char author2[] = "z";
    int test2 = AuthorInList(author2, node0);
    printf("test présence author: %i\n", test2);

    int taille= 6;
    Dijkstra(6,node0,taille);
    printDistance(6,node0);

    freeListAdj(node0);
}

int main(){
    testCreateListeAdj();
    return 0;
}



