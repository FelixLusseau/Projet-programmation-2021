#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "program.h"
#include "fonctionsMatrice.h"
#include "baseParser.h"
#include "readBinary.h"

extern int interruptFlag;

unsigned hash(unsigned char *str)
{
    unsigned hash = 5381;
    int c;
    while ((c = *str++))
        hash = ((hash << 5) + hash) + c;
    hash %= 50000000;
    //printf("%u\n", hash);
    return hash;
}

int AuthorInListHash(char *author, node **hashTable)
{
    unsigned int authorHashed = hash((unsigned char *)author);
    if (hashTable[authorHashed] == NULL)
        return -1;
    else
        return hashTable[authorHashed]->nodeNumber;
}

node *GoToNodeHash(node ** hashTable, unsigned int hash)
{
    return hashTable[hash];
}

void appendEdgeSousHash(edge *newEdge,int n1,edge *edge0){
    edge *currentEdge=edge0;
    edge *inter;
    
    while(currentEdge->linkNode->nodeNumber<n1 && currentEdge->nextEdge!=NULL){
        inter=currentEdge;
        currentEdge=currentEdge->nextEdge;
    }

    int index=currentEdge->linkNode->nodeNumber;

    if(currentEdge->nextEdge==NULL){
        if(index>n1){
            inter->nextEdge=newEdge;
            newEdge->nextEdge=currentEdge;
            newEdge->edgeNumber=inter->edgeNumber+1;
            currentEdge->edgeNumber=newEdge->edgeNumber+1;
        }
        else{
            currentEdge->nextEdge=newEdge;
            newEdge->nextEdge=NULL;
            newEdge->edgeNumber=currentEdge->edgeNumber+1;
        }
    }
    else{
        if(index==n1){
            newEdge->nextEdge=currentEdge->nextEdge;
            currentEdge->nextEdge=newEdge;
            newEdge->edgeNumber=currentEdge->edgeNumber+1;
            inter=currentEdge;
            currentEdge=newEdge;
        }
        else{
            inter->nextEdge=newEdge;
            newEdge->nextEdge=currentEdge;
            newEdge->edgeNumber=inter->edgeNumber+1;
            inter=newEdge;
        }
        while(currentEdge->nextEdge!=NULL){
            currentEdge->edgeNumber=inter->edgeNumber+1;
            inter=currentEdge;
            currentEdge=currentEdge->nextEdge;
        }
        currentEdge->edgeNumber=inter->edgeNumber+1;
    }
}
int appendEdgeHash(int n1, unsigned int hash1, int n2, unsigned int hash2, node *node0, node ** hashTable)
{   edge *edge0=node0->nodeEdge;
    node *Node1=GoToNodeHash(hashTable, hash1);
    node *Node2=GoToNodeHash(hashTable, hash2);
    //printf("author 1 : %s\n", Node1->author);
    //printf("author 2 : %s\n", Node2->author);
    if((n1==0 || n2==0) && edge0->indexNode==-1){
        if(n1==0){
            edge0->indexNode=n2;
            edge *newEdge2 = (edge *)malloc(sizeof(edge));
            if(newEdge2==NULL){
                fprintf(stderr, "appendEdge:erreur malloc edge = NULL");
            }
            newEdge2->indexNode=n1;
            newEdge2->linkNode=Node2;
            appendEdgeSousHash(newEdge2,n2,edge0);
        }
        else{
            edge0->indexNode=n1;
            edge *newEdge1 = (edge *)malloc(sizeof(edge));
            if(newEdge1==NULL){
                fprintf(stderr, "appendEdge:erreur malloc edge = NULL");
            }
            newEdge1->indexNode=n2;
            newEdge1->linkNode=Node1;
            appendEdgeSousHash(newEdge1,n1,edge0);
        }
        return 0;
    }

    edge *newEdge1 = (edge *)malloc(sizeof(edge));
     if(newEdge1==NULL){
        fprintf(stderr, "appendEdge:erreur malloc edge = NULL");
    }
    newEdge1->indexNode=n2;
    newEdge1->linkNode=Node1;
    newEdge1->edgeNumber=0;
    edge *newEdge2 = (edge *)malloc(sizeof(edge));
     if(newEdge2==NULL){
        fprintf(stderr, "appendEdge:erreur malloc edge = NULL");
    }
    newEdge2->indexNode=n1;
    newEdge2->linkNode=Node2;
    newEdge2->edgeNumber=0;

    if(Node1->nodeEdge==NULL){
        Node1->nodeEdge=newEdge1;
    }
    appendEdgeSousHash(newEdge1,n1,edge0);

    if(Node2->nodeEdge==NULL){
        Node2->nodeEdge=newEdge2;
    }
    appendEdgeSousHash(newEdge2,n2,edge0);
    
    node *currentNode;
    if(n1<n2){
        currentNode=Node1;
    }
    else{
        currentNode=Node2;
    }
    while(currentNode->nextNode!=NULL){
        if(currentNode->nodeEdge!=NULL){
            currentNode->indexEdge=currentNode->nodeEdge->edgeNumber;
        }
        currentNode=currentNode->nextNode;
    }
    if(currentNode->nodeEdge!=NULL){
        currentNode->indexEdge=currentNode->nodeEdge->edgeNumber;
    }
    return 0;
}

node *DoListAdjDeBinHash(options_t *option, int *taille)
{
    printf("************Debut DoListAdjDeBinHash************\n");
    node **hashTable = malloc(50000000 * sizeof(unsigned int) * sizeof(char *));
    if (hashTable == NULL)
        return NULL;
    /* for (int i = 0; i < 50000000; i++)
        hashTable[i] = NULL; */
    *taille = 0;
    structureBase_t Entree;
    initStructure(&Entree, 0);
    Entree = readEntryBin(option, -1);

    if (Entree.author[0] == NULL)
    {
        printf("Erreur 1er livre author[0]=NULL");
        return NULL;
    }
    node *node0 = CreateListAdj(Entree.author[0]);
    int n1 = 0;
    int n2 = 0;
    node *end = node0;
    for (int k = 1; k < Entree.authornb; k++)
    {
        char *author1 = Entree.author[k];
        unsigned int hash1 = hash((unsigned char *)author1);
        n1 = AuthorInListHash(author1, hashTable);
        if (n1 == -1)
        {
            end = appendNode(author1, end);
            hashTable[hash1]=end;
            *taille += 1;
            n1 = *taille;
        }
        for (int i = k + 1; i < Entree.authornb; i++)
        {
            char *author2 = Entree.author[i];
            unsigned int hash2 = hash((unsigned char *)author2);
            n2 = AuthorInListHash(author2, hashTable);
            if (n2 == -1)
            {
                end = appendNode(author2, end);
                hashTable[hash2]=end;
                *taille += 1;
                n2 = *taille;
            }
            appendEdgeHash(n1, hash1, n2, hash2, node0, hashTable);
        }
    }
    int L[100];
    unsigned int LH[100];
    while (Entree.authornb != 0)
    {
        if (interruptFlag == 1)
        {
            break;
        }
        if (Entree.authornb > 1)
        {
            L[0] = -1;
            int index = 0;
            for (int k = 0; k < Entree.authornb; k++)
            {
                char *author1 = Entree.author[k];
                unsigned int hash1 = hash((unsigned char *)author1);
                n1 = AuthorInListHash(author1, hashTable);
                if (n1 == -1)
                {
                    end = appendNode(author1, end);
                    hashTable[hash1] = end;
                    *taille += 1;
                    n1 = *taille;
                }
                L[index] = n1;
                LH[index]=hash1;
                index++;
                L[index] = -1;
            }
            for (int i = 0; L[i] > -1 && i < 100; i++)
            {
                for (int k = i + 1; L[k] > -1 && k < 100; k++)
                {
                    appendEdgeHash(L[i], LH[i], L[k], LH[k], node0, hashTable);
                }
            }
        }
        Entree = readEntryBin(option, -1);
    }
    /* for (int m=0; m<50000000; m++){
        if (hashTable[m]!=NULL){
            printf("%p\n", hashTable[m]);
            printf("%s\n", hashTable[m]->author);
        }
    } */
    printf("************Fin DoListAdjDeBinHash************\n");
    free(hashTable);
    return node0;
}