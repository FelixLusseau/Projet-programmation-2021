#ifndef FONCTIONSMATRICE_H
#define FONCTIONSMATRICE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "program.h"

//Structure pour faire une liste adjacente.
typedef struct node{
    char author[100];
    int nodeNumber;
    int distance;
    int flag;
    struct edge *nodeEdge;
    struct node *nextNode;
}node;

typedef struct edge{
    struct node *linkNode;
    struct node *otherNode;
    struct edge *nextEdge;
}edge;

node *CreateListAdj();

node *GoToNode(int n, node *node0);

node *GoToEndNode(node *node0);

edge * GoToEdge(int n, node *node0);

node * appendNode(char * author,node *end);

/** Prend le numéros de sommets liés et augmente le graphe.*/
int appendEdge(int n1,int n2,node *node0);

void freeEdge(node *currentNode);

void freeListAdj(node *node0);

void printListAdj(node *node0);

void printListNode(node * node0);

void printListEdge(node * node0);

/**test la présence de l'auteur dans la liste d'adjacence
 * renvoi le numéros de son sommet si il est présent
 * renvoi -1 sinon
*/
int AuthorInList(char *author, node *node0);

/** prend une ligne d'auteurs et append le graphe*/ 
void AuthorSAppend(char **authorS,node *node0,int*taille);

node* DoListAdjDeBin(options_t *option,int *taille);

/**calcul de la plus petite distance par rapport à un author*/
void Dijkstra(int n1,node *node0,int taille);

void printDistance(int n1,node *node0);

#endif