#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "baseParser.h"
#include "fonctionsMatrice.h"
#include "io-utils.h"
#include "program.h"
#include "readFunctions.h"

extern int interruptFlag;

unsigned hash(unsigned char *str) {
    unsigned hash = 5381;
    int c;
    while ((c = *str++))
        hash = ((hash << 5) + hash) + c;
    hash %= 50000000;
    // printf("%u\n", hash);
    return hash;
}

unsigned char *unhash(unsigned int authorHashed) {
    (void)authorHashed;
    return (unsigned char *)"ok";
}

int AuthorInListHash(char *author, node **hashTable) {
    unsigned int authorHashed = hash((unsigned char *)author);
    if (hashTable[authorHashed] == NULL)
        return -1;
    else
        return hashTable[authorHashed]->nodeNumber;
}

node *GoToNodeHash(node **hashTable, unsigned int hash) {
    return hashTable[hash];
}

int appendEdgeHash(unsigned int hash1, unsigned int hash2, node **hashTable) {
    node *Node1 = GoToNodeHash(hashTable, hash1);
    node *Node2 = GoToNodeHash(hashTable, hash2);

    edge *newEdge1 = (edge *)malloc(sizeof(edge));
    if (newEdge1 == NULL) {
        printf("appendEdge:erreur malloc edge = NULL");
    }

    edge *newEdge2 = (edge *)malloc(sizeof(edge));
    if (newEdge2 == NULL) {
        printf("appendEdge:erreur malloc edge = NULL");
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

    newEdge2->otherNode = Node1;
    newEdge2->linkNode = Node2;
    if (Node2->nodeEdge == NULL) {
        newEdge2->nextEdge = NULL;
        Node2->nodeEdge = newEdge2;
    } else {
        newEdge2->nextEdge = Node2->nodeEdge;
        Node2->nodeEdge = newEdge2;
    }

    return 0;
}

node *DoListAdjDeBinHash(options_t *option, int *taille, node **hashTable) {
    int nbEntries = readEntireBin(option, 0);
    int nbrarrete = 0;
    printf("\n*************************************Debut de la fonction "
           "graphe*************************************\n\n");
    printf("\33[?25l");

    *taille = 0;
    structureBase_t Entree;
    initStructure(&Entree, 0);
    Entree = readEntryBin(option, -1);

    if (Entree.author[0] == NULL) {
        printf("Erreur 1er livre author[0]=NULL");
        return NULL;
    }
    node *node0 = CreateListAdj(Entree.author[0]);
    int n1 = 0;
    int n2 = 0;
    node *end = node0;
    /* première boucle si il y a des auteur après author n°0 */
    for (int k = 1; k < Entree.authornb; k++) {
        char *author1 = Entree.author[k];
        // printf("%s\n", author1);
        unsigned int hash1 = hash((unsigned char *)author1);
        n1 = AuthorInListHash(author1, hashTable);
        if (n1 == -1) {
            end = appendNode(author1, end);
            hashTable[hash1] = end;
            *taille += 1;
            n1 = *taille;
        }
        for (int i = k + 1; i < Entree.authornb; i++) {
            char *author2 = Entree.author[i];
            // printf("%s\n", author2);
            unsigned int hash2 = hash((unsigned char *)author2);
            n2 = AuthorInListHash(author2, hashTable);
            if (n2 == -1) {
                end = appendNode(author2, end);
                hashTable[hash2] = end;
                *taille += 1;
                n2 = *taille;
            }
            appendEdgeHash(hash1, hash2, hashTable);
        }
    }
    int L[500];
    unsigned int LH[100];
    int curseur = 1;
    while (Entree.authornb != 0) {
        // printf("\33[?25l\rcurseur:%i\33[?25h", curseur);
        if (interruptFlag == 1) {
            printf("\33[?25h");
            break;
        }
        if (Entree.authornb >= 1) {
            L[0] = -1;
            int index = 0;
            //on récupère les numéros des co auteur de l'article et on les mets dans une liste
            for (int k = 0; k < Entree.authornb; k++) {
                char *author1 = Entree.author[k];
                // printf("%s\n", author1);
                unsigned int hash1 = hash((unsigned char *)author1);
                n1 = AuthorInListHash(author1, hashTable);
                if (n1 == -1) {
                    end = appendNode(author1, end);
                    hashTable[hash1] = end;
                    // printf("%p\n", hashTable[hash1]);
                    *taille += 1;
                    n1 = *taille;
                }
                // printf("%s\n", hashTable[hash1]->author);
                L[index] = n1;
                LH[index] = hash1;
                index++;
                L[index] = -1;
            }
            //on utilise la liste pour append le graphe
            for (int i = 0; L[i] > -1 && i < 100; i++) {
                for (int k = i + 1; L[k] > -1 && k < 100; k++) {
                    appendEdgeHash(LH[i], LH[k], hashTable);
                    nbrarrete++;
                }
            }
        }
        Entree = readEntryBin(option, -1);
        curseur++;
        if (curseur % 50000 == 0) {
            progressBar(curseur * 100 / nbEntries);
        }
    }
    // printf("hash nbr arret:%i\n",nbrarrete*2);
    progressBar(100);
    printf("\33[?25h");
    printf("\n\n**************************************Fin de la fonction "
           "graphe**************************************\n\n");
    option->action[ACTION_MAT] = DONE_SUCCESSFULLY;
    return node0;
}