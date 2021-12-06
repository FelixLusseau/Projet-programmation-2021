#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "program.h"
#include "argsParser.h"
#include "baseParser.h"
#include "io-utils.h"
#include "fonctionsMatrice.h"



void testCreateListeAdj(void)
{
    char c0[] = "author0";

    node *node0 = CreateListAdj(c0);

    char *c1 = "ab";
    char *c2 = "bc";
    char *c3 = "cd";
    char *c4 = "de";
    char *c5 = "ef";
    char *c6 = "fg";

    appendNode(c1, node0);
    appendNode(c2, node0);
    appendNode(c3, node0);
    appendNode(c4, node0);
    appendNode(c5, node0);
    appendNode(c6, node0);

    appendEdge(5, 1, node0);
    appendEdge(0, 3, node0);
    appendEdge(3, 2, node0);
    appendEdge(6, 5, node0);
    appendEdge(6, 3, node0);
    appendEdge(6, 2, node0);

    printListNode(node0);
    printListEdge(node0);

    char author[] = "de";
    int test = AuthorInList(author, node0);
    printf("test présence author: %i\n", test);

    char author2[] = "z";
    int test2 = AuthorInList(author2, node0);
    printf("test présence author: %i\n", test2);

    freeListAdj(node0);
}




