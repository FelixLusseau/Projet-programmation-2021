#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "baseParser.h"
#include "fonctionsMatrice.h"
#include "io-utils.h"
#include "program.h"
#include "readFunctions.h"

extern int interruptFlag;

unsigned hash(unsigned char *str, int pr) {
    unsigned hash = pr;
    int c;
    while ((c = *str++))
        hash = ((hash << 5) + hash) + c;
    hash %= HT_SIZE;
    // printf("%u\n", hash);
    return hash;
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

void sousAppendEdge(node *Node1, node *Node2) {
    edge *newEdge1 = (edge *)malloc(sizeof(edge));
    if (newEdge1 == NULL) {
        fprintf(stderr, "appendEdge:erreur malloc edge = NULL");
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

    sousAppendEdge(Node1, Node2);
    sousAppendEdge(Node2, Node1);
}

node *sousListeAdj(node *end, int *taille, structureBase_t *Entree,
                   node **hashTable) {
    int pr[4] = {pr1, pr2, pr3, pr4};
    int n1 = 0;
    int L[500];
    L[0] = -1;
    unsigned int LH[100];
    int index = 0;
    /*on récupère les numéros des co auteur de l'article et on les mets
     dans une liste*/
    for (int k = 0; k < Entree->authornb; k++) {
        char *author1 = Entree->author[k];
        unsigned int hash1 = 0;
        for (int p = 0; p < 4; p++) {
            hash1 = hash((unsigned char *)author1, pr[p]);
            n1 = AuthorInListHash(author1, hashTable, pr[p]);
            if (n1 == -1) {
                end = appendNode(author1, end);
                hashTable[hash1] = end;
                *taille += 1;
                n1 = *taille;
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

    // on utilise la liste pour append le graphe
    for (int i = 0; L[i] > -1 && i < 100; i++) {
        for (int k = i + 1; L[k] > -1 && k < 100; k++) {
            appendEdgeHash(LH[i], LH[k], hashTable);
        }
    }
    return end;
}

node *DoListAdjDeBinHash(options_t *options, int *taille, node **hashTable) {
    int nbEntries = readEntireBin(options, 0);
    printf("\n*************************************Debut de la fonction "
           "graphe*************************************\n\n");
    printf("\33[?25l");

    *taille = 0;
    int16_t precAuthornb = 0;
    structureBase_t Entree;
    initStructure(&Entree, 0);
    readStructure(options, &Entree, precAuthornb);
    precAuthornb = Entree.authornb;

    if (Entree.author[0] == NULL) {
        printf("Erreur 1er livre author[0]=NULL");
        return NULL;
    }
    node *node0 = CreateListAdj(Entree.author[0]);
    if (node0 == NULL)
        return NULL;
    node *end = node0;
    if (Entree.authornb > 1) {
        end = sousListeAdj(end, taille, &Entree, hashTable);
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
            end = sousListeAdj(end, taille, &Entree, hashTable);
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
    printf("\n\n**************************************Fin de la fonction "
           "graphe**************************************\n\n");
    return node0;
}

int printAuthorAtDist(options_t *options, node *node0) {
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