#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "analyseGraph.h"
#include "baseParser.h"
#include "makeGraph.h"

#include "program.h"
#include "readBin.h"
#include "tps_unit_test.h"

extern int interruptFlag;

void printListAdj(node *node0) {
    printListNode(node0);
    printListEdge(node0);
}

void printListNode(node *node0) {
    if (node0 == NULL) {
        exit(ERROR_NODE_EQ_NULL);
    }
    node *currentNode = node0;
    printf("\n");
    printf("List of nodes:\n");

    while (currentNode->nextNode != NULL) {

        if (interruptFlag == 1) {
            break;
        }
        printf("  %i:%s\n", currentNode->nodeNumber, currentNode->author);
        currentNode = currentNode->nextNode;
    }

    printf("  %i:%s\n", currentNode->nodeNumber, currentNode->author);
}

void printListEdgeOfAuthor(node *node0) {
    edge *currentEdge = node0->nodeEdge;
<<<<<<< HEAD
    // printf(" %s -> %s\n", currentEdge->linkNode->author,
    //      currentEdge->otherNode->author);
=======
>>>>>>> a27fad69c7153a8708a9abcc2b6d485dd25bbbb6

    while (currentEdge->nextEdge != NULL) {
        printf(" %s -> %s\n", currentEdge->linkNode->author,
               currentEdge->otherNode->author);
        currentEdge = currentEdge->nextEdge;
    }

    printf(" %s -> %s\n", currentEdge->linkNode->author,
           currentEdge->otherNode->author);
}

void printListEdge(node *node0) {
    node *currentNode = node0;
    edge *currentEdge = currentNode->nodeEdge;
    printf("\n");
    printf("List of edges:\n");

    while (currentNode->nextNode != NULL) {

        if (interruptFlag == 1) {
            break;
        }
        currentEdge = currentNode->nodeEdge;
        if (currentEdge != NULL) {
            printListEdgeOfAuthor(currentNode);
        }
        currentNode = currentNode->nextNode;
    }

    if (currentEdge != NULL) {
        printListEdgeOfAuthor(currentNode);
    }
    printf("\n");
}

node *createList(void) {
    node *node0 = (node *)malloc(sizeof(node));
    if (node0 == NULL) {
        fprintf(stderr, "creatListAdj:erreur malloc node = NULL");
        return NULL;
    }
    node0->nodeNumber = 0;
    node0->flag = 0;
    node0->nextNode = NULL;
    node0->distance = -1;
    node0->nodeEdge = NULL;

    return node0;
}
node *popListeEdge(node *startList) {
    node *node0 = startList->nodeEdge->linkNode;
    edge *inter = startList->nodeEdge;
    startList->nodeEdge = startList->nodeEdge->nextEdge;
    free(inter);
    return node0;
}
error_t removeEdgeListe(node *startList, node *nodeToRemove) {
    if (startList->nodeEdge->linkNode->nodeNumber == nodeToRemove->nodeNumber) {
        edge *inter = startList->nodeEdge;
        startList->nodeEdge = startList->nodeEdge->nextEdge;
        free(inter);
        return ERROR_DIJKSTRA;
    }
    edge *currentEdge = startList->nodeEdge;
    edge *previousEdge = startList->nodeEdge;

    while (currentEdge->linkNode->nodeNumber != nodeToRemove->nodeNumber) {
        previousEdge = currentEdge;
        currentEdge = currentEdge->nextEdge;
    }

    previousEdge->nextEdge = currentEdge->nextEdge;
    free(currentEdge);
    return OK;
}
edge *appendListeEdge(edge *endListe, node *newNode, node *startList) {
    edge *newListe = (edge *)malloc(sizeof(edge));
    if (newListe == NULL) {
        fprintf(stderr, "appendListeEdge:erreur malloc node = NULL ");
        exit(1);
    }
    newListe->linkNode = newNode;
    newListe->nextEdge = NULL;

    if (startList->nodeEdge == NULL) {
        startList->nodeEdge = newListe;
        return newListe;
    }
    endListe->nextEdge = newListe;
    return newListe;
}

error_t dijkstra(node *node1, node *node2, int size) {
    /* flag to know if we want the distance between 2 node or
    if we do all of the connected component*/
    int flagGraph = 0;
    if (node2 == NULL) {
        flagGraph = 1;
    }

    // distance = -1 means infinity

    if (node1->nodeEdge == NULL) {
        printf("node:%s isolated \n distance:-1\n", node1->author);
        return 0;
    }
    node *currentNode = node1;
    currentNode->distance = 0;
    edge *currentEdge = currentNode->nodeEdge;
    node *neighbor = currentEdge->otherNode;
    int flag = neighbor->distance;

    /* list of nodes not marked (flag=0) with a distance changed from infinity
    (!=-1), it is used to find the minimum wich is the next currentNode*/
    node *listDistance = createList();
    listDistance->distance = 1;
    edge *endListe = appendListeEdge(NULL, currentNode, listDistance);
    int k = 0;

    printf("******************************************** Start dijkstra"
           " *******************************************\n\n");
    while (k < size) {

        if (interruptFlag == 1) {
            printf("\33[?25h");
            break;
        }

        currentEdge = currentNode->nodeEdge;
        neighbor = currentEdge->otherNode;

        /* explore neigbour and change their distance*/
        while (currentEdge != NULL) {

            flag = neighbor->distance;
            if (neighbor->distance == -1 ||
                (neighbor->distance > (currentNode->distance + 1) &&
                 neighbor->flag == 0)) {

                neighbor->distance = currentNode->distance + 1;

                /* if the e neighbor had a distance=-1 before
                then he wasn't in listDistance */
                if (flag == -1) {
                    endListe =
                        appendListeEdge(endListe, neighbor, listDistance);
                    listDistance->distance += 1;
                }
            }

            if (currentEdge->nextEdge == NULL) {
                break;
            }
            currentEdge = currentEdge->nextEdge;
            neighbor = currentEdge->otherNode;
        }

        currentNode->flag = 1;
        /* remove currentNode from the list because it is marked */
        removeEdgeListe(listDistance, currentNode);
        listDistance->distance -= 1;

        /*listDistance=NULL, end of the connected component*/
        if (listDistance->nodeEdge == NULL) {
            break;
        }
        /* if node2 marked, end of drijkstra */
        if (flagGraph == 0 && node2->flag == 1) {
            break;
        }

        // listDistance != NULL, search for the minimum
        else {
            edge *listeEdge = listDistance->nodeEdge;
            node *minNode = listeEdge->linkNode;
            int minDistance = minNode->distance;

            while (1) {

                /* if distance = previous minimum, then it is a minimum */
                if (listeEdge->linkNode->distance == currentNode->distance) {
                    minNode = listeEdge->linkNode;
                    break;
                }

                if (listeEdge->linkNode->distance < minDistance) {
                    minNode = listeEdge->linkNode;
                    minDistance = minNode->distance;
                }
                if (listeEdge->nextEdge == NULL) {
                    break;
                }
                listeEdge = listeEdge->nextEdge;
            }

            currentNode = minNode;
        }
        k++;
    }
    if (flagGraph == 0) {
        printf("Distance : %i\n", node2->distance);
    }

    freeListAdj(listDistance, 0);
    printf("\n********************************************* End dijkstra"
           " ********************************************\n\n");
    return OK;
}

void printDistances(options_t *options, node *node0) {
    node *currentNode = node0;
    printf("Distances : \n");
    while (currentNode != NULL) {
        if (currentNode->distance != -1) {
            printf("%s - %s : %i\n", options->authorNames[0],
                   currentNode->author, currentNode->distance);
        }
        currentNode = currentNode->nextNode;
    }
}

error_t shortestPath(node *Node1, node *Node2, int size) {
    int exitCode = dijkstra(Node1, Node2, size);
    if (exitCode)
        return exitCode;
    int len;

    printf("\nShortest path between %s and %s :\n\n", Node1->author,
           Node2->author);
    node *currentNode = Node2;
    int distance = currentNode->distance;
    if (distance == -1) {
        fprintf(stderr, "No path between %s and %s\n", Node1->author,
                Node2->author);
        return ERROR_PATH;
    }

    edge *currentEdge = currentNode->nodeEdge;
    node *minneighbor = currentEdge->otherNode;

    while (currentNode->nodeNumber != Node1->nodeNumber) {

        len = (50 - strlen(currentNode->author)) / 2;
        printf("%*s%s%*s\n", len, "", currentNode->author, len, "");
        printf("%*s\33[0;32m|\33[0m%*s\n", 25, "", 25, "");

        while (currentEdge != NULL) {

            if (currentEdge->linkNode->flag == 1 &&
                currentEdge->otherNode->distance < minneighbor->distance) {
                minneighbor = currentEdge->otherNode;
            }
            currentEdge = currentEdge->nextEdge;
        }

        currentNode = minneighbor;
        currentEdge = currentNode->nodeEdge;
    }

    len = (50 - strlen(currentNode->author)) / 2;
    printf("%*s%s%*s\n", len, "", currentNode->author, len, "");
    printf("\nDistance between %s and %s : %i\n", Node1->author, Node2->author,
           distance);
    return OK;
}

int exploreGraph(node *node0) {
    int nbrNode = 0;
    node0->flag = 1;

    if (node0->nodeEdge == NULL) {
        return 0;
    }
    edge *currentEdge = node0->nodeEdge;
    node *neighbor = currentEdge->otherNode;

    node *debutListe = createList();
    edge *endListe = appendListeEdge(NULL, node0, debutListe);

    while (debutListe->nodeEdge != NULL) {
        nbrNode += 1;
        node0 = popListeEdge(debutListe);
        currentEdge = node0->nodeEdge;
        neighbor = currentEdge->otherNode;

        while (1) {
            if (neighbor->flag == 0) {
                neighbor->flag = 1;
                endListe = appendListeEdge(endListe, neighbor, debutListe);
            }
            if (currentEdge->nextEdge == NULL) {
                break;
            }
            currentEdge = currentEdge->nextEdge;
            neighbor = currentEdge->otherNode;
        }
    }

    free(debutListe);
    return nbrNode;
}

void nbrConnectedComponent(node *node0) {
    node *currentNode = node0;
    int nbrConnected = 0;

    while (currentNode != NULL) {
        if (interruptFlag == 1) {
            break;
        }
        if (currentNode->flag == 0) {
            exploreGraph(currentNode);
            nbrConnected += 1;
        }
        currentNode = currentNode->nextNode;
    }

    printf("Connected components : %i", nbrConnected);
}

node *verifyAuthorHash(options_t *options, node **hashTable, int author0or1) {
    int pr[4] = {pr1, pr2, pr3, pr4};
    for (int p = 0; p < 4; p++) {
        if (strcmp(hashTable[hash((unsigned char *)
                                      options->authorNames[author0or1],
                                  pr[p])]
                       ->author,
                   options->authorNames[author0or1]) == 0) {
            return hashTable[hash(
                (unsigned char *)options->authorNames[author0or1], pr[p])];
        }
    }
    return NULL;
}

error_t printAuthorAtDist(options_t *options, node *node0) {
    if (node0 == NULL) {
        return ERROR_NODE_EQ_NULL;
    }
    node *currentNode = node0;
    printf("\nAuthors at distance %i of %s : \n", options->N,
           options->authorNames[0]);

    while (currentNode->nextNode != NULL) {
        if (currentNode->distance == options->N) {
            printf(" - %s\n", currentNode->author);
        }
        currentNode = currentNode->nextNode;
        if (interruptFlag == 1)
            break;
    }
    return OK;
}