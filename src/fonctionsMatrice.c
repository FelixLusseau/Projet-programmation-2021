#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "program.h"
#include "fonctionsMatrice.h"

node *CreateListAdj(char *author){
    node *node0 = (node *)malloc(sizeof(node));
    if(node0==NULL){
        printf("creatListAdj:erreur malloc node = NULL");
        return NULL;
    }
    node0->author=author;
    node0->nodeNumber =0;
    node0->indexEdge=0;
    node0->nextNode=NULL;
    node0->nodeEdge=NULL;

    edge *edge0=(edge *)malloc(sizeof(edge));
    if(edge0==NULL){
        printf("creatListAdj:erreur malloc edge = NULL");
        return NULL;
    }
    edge0->linkNode=node0;
    edge0->indexNode=-1;
    edge0->nextEdge=NULL;
    edge0->edgeNumber=0;
    node0->nodeEdge=edge0;
    node0->indexEdge=0;

    return node0;
}
node *GoToNode(int n, node *node0)
{
    node *currentNode = node0;
    for (int k = 0; currentNode->nodeNumber < n; k++)
    {
        if(currentNode->nextNode==NULL){
            printf("dépassement de list:index trop grand");
            return currentNode;
        }
        currentNode = currentNode->nextNode;
    }
    return currentNode;
}
node *GoToEndNode(node *node0){
    if(node0==NULL){
        return NULL;
    }
    node *currentNode = node0;
    while(currentNode->nextNode!=NULL)
    {
        currentNode = currentNode->nextNode;
    }
    return currentNode;
}

edge *GoToEndEdge(node *node0){
    edge *currentEdge=node0->nodeEdge;
    if(currentEdge==NULL){
        return NULL;
    }
    while(currentEdge->nextEdge!=NULL)
    {
        currentEdge = currentEdge->nextEdge;
    }
    return currentEdge;
}
edge * GoToEdge(int n, node *node0){
    node *currentNode = GoToNode(n,node0);
    if(currentNode->nodeEdge==NULL){
        return NULL;
    }
    return currentNode->nodeEdge;
}

void appendNode(node *node0,char * author){
    node *newNode=malloc(sizeof(node));
    node *currentNode = GoToEndNode(node0);
    int n=currentNode->nodeNumber;
    currentNode->nextNode=newNode;
    newNode->nodeNumber=n+1;
    newNode->nextNode=NULL;
    newNode->author=author;

    edge *currentEdge=GoToEndEdge(node0);
    edge *newEdge=(edge *)malloc(sizeof(edge));
    newEdge->indexNode=-1;
    newEdge->edgeNumber=currentEdge->edgeNumber+1;
    newEdge->linkNode=newNode;
    newEdge->nextEdge=NULL;

    currentEdge->nextEdge=newEdge;
    newNode->nodeEdge=newEdge;
    newNode->indexEdge=newEdge->edgeNumber;
}

void appendEdgeSous(edge *edge1,int n1,int n2, node * Node1){
    if(edge1->indexNode>=0){
        edge *newEdge1=(edge *)malloc(sizeof(edge));
        newEdge1->indexNode=n2;
        newEdge1->linkNode=Node1;
        edge *inter=edge1;
        int Number=0;
        while(edge1->linkNode->nodeNumber==n1 && edge1->nextEdge!=NULL){
            inter=edge1;
            edge1=edge1->nextEdge;
        }
        Number=edge1->edgeNumber;
        if(edge1->nextEdge==NULL){
            if(edge1->linkNode->nodeNumber==n1){
                edge1->nextEdge=newEdge1;
                newEdge1->nextEdge=NULL;
                newEdge1->edgeNumber=Number+1;
            }
            else{
                newEdge1->nextEdge=edge1;
                inter->nextEdge=newEdge1;
                newEdge1->edgeNumber=Number;
                edge1->edgeNumber=Number;
            }
        }
        else{
            newEdge1->nextEdge=edge1;
            inter->nextEdge=newEdge1;
            newEdge1->edgeNumber=Number;
            while(edge1->nextEdge!=NULL){
                Number++;
                edge1->edgeNumber=Number;
                edge1=edge1->nextEdge;
            }
            edge1->edgeNumber=Number+1;
        }
        
    }
    else{
        edge1->indexNode=n2;
    }
}
void appendEdge(int n1,int n2,node *node0)
{
    node *Node1=GoToNode(n1,node0);
    edge *edge1=GoToEdge(n1,node0);
    node *Node2=GoToNode(n2,node0);
    edge *edge2=GoToEdge(n2,node0);
    appendEdgeSous(edge1,n1,n2,Node1);
    appendEdgeSous(edge2,n2,n1,Node2);
    node *currentNode=node0;
    while(currentNode->nextNode!=NULL){
        currentNode->indexEdge=currentNode->nodeEdge->edgeNumber;
        currentNode=currentNode->nextNode;
    }
    currentNode->indexEdge=currentNode->nodeEdge->edgeNumber;
    
}

void freeListAdj(node *node0){
    edge *currentEdge=node0->nodeEdge;
    node *currentNode=node0;
    edge *inter;
    node *interN;
    while(currentNode->nextNode!=NULL){
        interN=currentNode;
        currentNode=currentNode->nextNode;
        free(interN);
    }
    free(currentNode);
    while(currentEdge->nextEdge!=NULL){
        inter=currentEdge;
        currentEdge=currentEdge->nextEdge;
        free(inter);
    }
    free(currentEdge);
}

void printListAdj(node *node0){
    node *currentNode=node0;
    edge *currentEdge=node0->nodeEdge;
    int n=currentNode->nodeNumber;
    while(currentNode->nextNode !=NULL){
        printf("nom sommet:%s  numéros sommet:%i\nliste edge:",currentNode->author,currentNode->nodeNumber);
        while(currentEdge->linkNode->nodeNumber == n && currentEdge->nextEdge!=NULL){
            printf("%i |",currentEdge->indexNode);
            currentEdge=currentEdge->nextEdge;
        }
        
        printf("\n");
        currentNode=currentNode->nextNode;
        currentEdge=currentNode->nodeEdge;
        n=currentNode->nodeNumber;
    }
    printf("nom sommet:%s  numéros sommet:%i\nliste edge:",currentNode->author,currentNode->nodeNumber);
        while(currentEdge->nextEdge!=NULL){
            printf("%i |",currentEdge->indexNode);
            currentEdge=currentEdge->nextEdge;
        }
        printf("%i |",currentEdge->indexNode);
        currentEdge=currentEdge->nextEdge;
        
        printf("\n");
}
void printListNode(node * node0){
    if(node0==NULL){
        exit(0);
    }
    node *currentNode=node0;
    printf("liste sommet:\n");
    while(currentNode->nextNode !=NULL){
        //printf("  %i: %s -> %i |",currentNode->nodeNumber,currentNode->author,currentNode->indexEdge);
        printf("  %i |",currentNode->indexEdge);
        currentNode=currentNode->nextNode;
    }
    //printf( "%i: %s -> %i |",currentNode->nodeNumber,currentNode->author,currentNode->indexEdge);
    printf("  %i |\n",currentNode->indexEdge);
}
void printListEdge(node * node0){
    edge *currentEdge=node0->nodeEdge;
    printf("liste arretes:\n");
    while(currentEdge->nextEdge !=NULL){
        //printf(" %i :%i -> %i|",currentEdge->edgeNumber, currentEdge->indexNode,currentEdge->linkNode->nodeNumber);
        printf(" %i |",currentEdge->indexNode);
        currentEdge=currentEdge->nextEdge;
    }
    //printf(" %i :%i -> %i|\n",currentEdge->edgeNumber,currentEdge->indexNode,currentEdge->linkNode->nodeNumber);
    printf(" %i |\n",currentEdge->indexNode);
}


int AuthorInList(char *author, node *node0)
{
    node *currentNode = node0;
    while (currentNode->nextNode != NULL)
    {
        if (strcmp(currentNode->author, author) == 0)
        {
            return currentNode->nodeNumber;
        }
        currentNode = currentNode->nextNode;
    }
    if (strcmp(currentNode->author, author) == 0){
        return currentNode->nodeNumber;
    }
    return -1;
}

/*
node * DoAdjacentList()
{
    //structureBase_t *infoArticle;
    node node0 = malloc(sizeof(node));
    node0.nodeNumber = 0;
    node0.indexEdge=0;
    //node *currentNode = node0;
    return node0;
    while (fread(infoArticle, sizeof(structureBase_t), 1, binaire) != 0)
    {
        for (int i = 0; i < 10; i++)
        {
            if (author[i] == NULL)
            {
                break;
            }
            int n = AuthorInList(author[i], node1);
            if (n == 0)
            {
                currentNumber += 1;
                node *newNode = malloc(sizeof(node));
                newNode->nodeName = author[i];
                newNode->nodeNumber = currentNumber;
                currentNode->nextNode = newNode;
                currentNode = newNode;
            }
            for (int k = 0; k < i; k++)
            {
                int n2 = AuthorInList(author[k], node1);
                appendEdges(n,n2);
            }
        }
    }
    return node1; 
}**/


