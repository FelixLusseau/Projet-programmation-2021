#ifndef FONCTIONSMATRICE_H
#define FONCTIONSMATRICE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "program.h"

//Structure pour faire une liste adjacente.
typedef struct node{
    int nodeNumber;
    int indexEdge;
    struct edge *nodeEdge;
    struct node *nextNode;
}node;

typedef struct edge{
    int indexNode;
    int edgeNumber;
    struct node *linkNode;
    struct edge *nextEdge;
}edge;

/*node * DoAdjacentList();*/

node *CreateListAdj();

node *GoToNode(int n, node *node0);

node *GoToEndNode(node *node0);


edge *GoToEndEdge(node *node0);

edge * GoToEdge(int n, node *node0);

void appendNode(node *node0, node *newNode);

void appendEdge(int n1,int n2,node *node0);

void freeListAdj(node *node0);

void printListAdj(node *node0);

void printListNode(node * node0);
void printListEdge(node * node0);

#endif