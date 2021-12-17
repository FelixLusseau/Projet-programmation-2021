#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "baseParser.h"
#include "fonctionsMatrice.h"
#include "program.h"
#include "readBinary.h"

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

int appendEdgeHash(unsigned int hash1,unsigned int hash2, node ** hashTable)
{
    node *Node1=GoToNodeHash(hashTable, hash1);
    node *Node2=GoToNodeHash(hashTable, hash2);
    
    edge *newEdge1 = (edge *)malloc(sizeof(edge));
     if(newEdge1==NULL){
        printf("appendEdge:erreur malloc edge = NULL");
    }

    edge *newEdge2 = (edge *)malloc(sizeof(edge));
     if(newEdge2==NULL){
        printf("appendEdge:erreur malloc edge = NULL");
    }
    
    newEdge1->otherNode = Node2;
    newEdge1->linkNode = Node1;
    if(Node1->nodeEdge==NULL){
        newEdge1->nextEdge = NULL;
        Node1->nodeEdge = newEdge1;
    }
    else{
        newEdge1->nextEdge = Node1->nodeEdge->nextEdge;
        Node1->nodeEdge = newEdge1;
    }


    newEdge2->otherNode=Node1;
    newEdge2->linkNode=Node2;
    if(Node2->nodeEdge==NULL){
        newEdge2->nextEdge = NULL;
        Node2->nodeEdge = newEdge1;
    }
    else{
        newEdge2->nextEdge = Node1->nodeEdge->nextEdge;
        Node2->nodeEdge = newEdge1;
    }

    return 0;
}

node *DoListAdjDeBinHash(options_t *option, int *taille) {
    int nbrarrete = 0;
    printf("************Debut DoListAdjDeBinHash************\n");
    node **hashTable = malloc(50000000 * sizeof(unsigned int) * sizeof(char *));
    if (hashTable == NULL)
        return NULL;
    for (int i = 0; i < 50000000; i++)
        hashTable[i] = NULL;
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
    // première boucle si il y a des auteur après author n°0
    for (int k = 1; k < Entree.authornb; k++) {
        char *author1 = Entree.author[k];
        unsigned int hash1 = hash((unsigned char *)author1);
        n1 = AuthorInListHash(author1, hashTable);
        // printf("n1 : %i\n", n1);
        if (n1 == -1) {
            end = appendNode(author1, end);
            hashTable[hash1] = end;
            *taille += 1;
            n1 = *taille;
        }
        for (int i = k + 1; i < Entree.authornb; i++) {
            char *author2 = Entree.author[i];
            unsigned int hash2 = hash((unsigned char *)author2);
            n2 = AuthorInListHash(author2, hashTable);
            // printf("n2 : %i\n", n2);
            if (n2 == -1) {
                end = appendNode(author2, end);
                hashTable[hash2] = end;
                *taille += 1;
                n2 = *taille;
            }
            appendEdgeHash( hash1, hash2, hashTable);
        }
    }
    // printf("OK");
    int L[100];
    unsigned int LH[100];
    int curseur = 0;
    while (Entree.authornb != 0) {
        printf("\33[?25l\rcurseur:%i\33[?25h", curseur);
        if (interruptFlag == 1) {
            break;
        }
        if (Entree.authornb > 1) {
            L[0] = -1;
            int index = 0;
            for (int k = 0; k < Entree.authornb; k++) {
                char *author1 = Entree.author[k];
                unsigned int hash1 = hash((unsigned char *)author1);
                n1 = AuthorInListHash(author1, hashTable);
                // printf("n1 : %i\n", n1);
                if (n1 == -1) {
                    end = appendNode(author1, end);
                    hashTable[hash1] = end;
                    *taille += 1;
                    n1 = *taille;
                    // printf("n1 = *taille : %i\n", n1);
                }
                L[index] = n1;
                LH[index] = hash1;
                index++;
                L[index] = -1;
            }
            for (int i = 0; L[i] > -1 && i < 100; i++){
                for (int k = i + 1; L[k] > -1 && k < 100; k++){
                    appendEdgeHash( LH[i], LH[k], hashTable);
                    nbrarrete++;
                    // printf("Li : %i Lk : %i\n", L[i], L[k]);
                }
            }
        }
        Entree = readEntryBin(option, -1);
        curseur++;
        /* if (curseur==1000)
            break; */
    }
    /* for (int m=0; m<50000000; m++){
        if (hashTable[m]!=NULL){
            printf("%p\n", hashTable[m]);
            printf("%s\n", hashTable[m]->author);
        }
    } */
    // printf("hash nbr arret:%i\n",nbrarrete*2);
    printf("************Fin DoListAdjDeBinHash************\n");
    free(hashTable);
    return node0;
}