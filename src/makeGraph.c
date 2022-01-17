#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "analyseGraph.h"
#include "baseParser.h"
#include "io-utils.h"
#include "program.h"
#include "readFunctions.h"
#include "tps_unit_test.h"

extern int interruptFlag;

unsigned hash(unsigned char *str, int pr) {
    unsigned hash = pr;
    int c;
    while ((c = *str++))
        hash = ((hash << 5) + hash) + c;
    hash %= HT_SIZE;
    return hash;
}

void CreateListAdj(char *author, node **hashTable, node *node0) {
    int k = 0;
    unsigned int hash1 = 0;
    hash1 = hash((unsigned char *)author, pr1);
    hashTable[hash1] = node0;
    while (author[k] != '\0') {
        node0->author[k] = author[k];
        k++;
    }
    node0->author[k] = '\0';
    node0->nodeNumber = 0;
    node0->flag = 0;
    node0->nextNode = NULL;
    // distance = -1 means infinity
    node0->distance = -1;
    node0->nodeEdge = NULL;
}

int AuthorInListHash(char *author, node **hashTable, int pr) {
    unsigned int authorHashed = hash((unsigned char *)author, pr);
    if (hashTable[authorHashed] == NULL)
        return -1;
    else if (hashTable[authorHashed] != NULL &&
             strcmp(author, hashTable[authorHashed]->author))
        return -2;
    else
        return hashTable[authorHashed]->nodeNumber;
}

node *GoToNodeHash(node **hashTable, unsigned int hash) {
    return hashTable[hash];
}

node *appendNode(char *author, node *end) {
    node *newNode = malloc(sizeof(node));
    if (newNode == NULL) {
        fprintf(stderr, "appendNode:error malloc node = NULL ");
    }
    node *currentNode = end;
    currentNode->nextNode = newNode;
    newNode->nextNode = NULL;
    newNode->nodeNumber = currentNode->nodeNumber + 1;
    // distance = -1 means infinity
    newNode->distance = -1;
    newNode->flag = 0;
    newNode->nodeEdge = NULL;
    int k = 0;
    while (author[k] != '\0') {
        newNode->author[k] = author[k];
        k++;
    }
    newNode->author[k] = '\0';
    return newNode;
}

void AppendEdge2(node *Node1, node *Node2) {
    edge *newEdge1 = (edge *)malloc(sizeof(edge));
    if (newEdge1 == NULL) {
        fprintf(stderr, "appendEdge:error malloc edge = NULL");
    }
    newEdge1->otherNode = Node2;
    newEdge1->linkNode = Node1;
    if (Node1->nodeEdge == NULL) {
        newEdge1->nextEdge = NULL;
        Node1->nodeEdge = newEdge1;
    } else {
        newEdge1->nextEdge = Node1->nodeEdge;
        Node1->nodeEdge = newEdge1;
    }
}
void appendEdgeHash(unsigned int hash1, unsigned int hash2, node **hashTable) {
    node *Node1 = GoToNodeHash(hashTable, hash1);
    node *Node2 = GoToNodeHash(hashTable, hash2);

    AppendEdge2(Node1, Node2);
    AppendEdge2(Node2, Node1);
}

node *ListeAdj2(node *end, int *size, structureBase_t *Entree,
                node **hashTable) {
    int pr[4] = {pr1, pr2, pr3, pr4};
    int n1 = 0;
    int L[500];
    L[0] = -1;
    unsigned int LH[100];
    int index = 0;
    /*the names of authors are converted to their number
    to put them in a list */
    for (int k = 0; k < Entree->authornb; k++) {
        char *author1 = Entree->author[k];
        unsigned int hash1 = 0;
        for (int p = 0; p < 4; p++) {
            hash1 = hash((unsigned char *)author1, pr[p]);
            n1 = AuthorInListHash(author1, hashTable, pr[p]);
            if (n1 == -1) {
                end = appendNode(author1, end);
                hashTable[hash1] = end;
                *size += 1;
                n1 = *size;
                break;
            } else if (n1 == -2) {
                continue;
            } else
                break;
        }
        L[index] = n1;
        LH[index] = hash1;
        index++;
        L[index] = -1;
    }

    // the list is now use to append the graphe
    for (int i = 0; L[i] > -1 && i < 100; i++) {
        for (int k = i + 1; L[k] > -1 && k < 100; k++) {
            appendEdgeHash(LH[i], LH[k], hashTable);
        }
    }
    return end;
}

error_t doListAdjHash(options_t *options, int *size, node **hashTable,
                      node *node0) {
    int nbEntries = readEntireBin(options, 0);
    printf("\n************************************* Start of the function "
           "graph *************************************\n\n");
    printf("\33[?25l");

    *size = 0;
    int16_t precAuthornb = 0;
    structureBase_t Entree;
    initStructure(&Entree, 0);
    readStructure(options, &Entree, precAuthornb);
    precAuthornb = Entree.authornb;

    if (Entree.author[0] == NULL) {
        printf("Error 1st book: author[0]=NULL");
        return ERROR_LIST;
    }

    CreateListAdj(Entree.author[0], hashTable, node0);
    if (node0 == NULL)
        return ERROR_LIST;
    node *end = node0;
    if (Entree.authornb > 1) {
        end = ListeAdj2(end, size, &Entree, hashTable);
    }

    readStructure(options, &Entree, precAuthornb);
    precAuthornb = Entree.authornb;
    int curseur = 1;

    while (Entree.authornb != 0) {

        if (interruptFlag == 1) {
            printf("\33[?25h");
            break;
        }
        if (Entree.authornb >= 1) {
            end = ListeAdj2(end, size, &Entree, hashTable);
        }
        readStructure(options, &Entree, precAuthornb);
        precAuthornb = Entree.authornb;
        curseur++;
        if (curseur % 50000 == 0) {
            progressBar(curseur * 100 / nbEntries);
        }
    }

    progressBar(100);
    printf("\33[?25h");
    printf("\n\n************************************** End of the function "
           "graph **************************************\n\n");
    return OK;
}

void freeEdge(node *currentNode) {
    edge *currentEdge = currentNode->nodeEdge;
    edge *inter;
    while (currentEdge->nextEdge != NULL) {
        inter = currentEdge;
        currentEdge = currentEdge->nextEdge;
        free(inter);
    }
    free(currentEdge);
}
void freeListAdj(node *node0, int print) {
    if (print)
        printf("\n\n************************************ Freeing the "
               "space **************************************\n\n");
    node *currentNode = node0;
    node *interN;
    while (currentNode->nextNode != NULL) {
        if (currentNode->nodeEdge != NULL) {
            freeEdge(currentNode);
        }
        interN = currentNode;
        currentNode = currentNode->nextNode;
        free(interN);
    }
    if (currentNode->nodeEdge != NULL) {
        freeEdge(currentNode);
    }
    free(currentNode);
}