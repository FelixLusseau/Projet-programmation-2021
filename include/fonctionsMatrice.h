#ifndef FONCTIONSMATRICE_H
#define FONCTIONSMATRICE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "program.h"

//Structure pour faire une liste adjacente.
typedef struct node{
    char author[50];
    int nodeNumber;
    int indexEdge;
    int flag;
    struct edge *nodeEdge;
    struct node *nextNode;
}node;

typedef struct edge{
    int indexNode;
    int edgeNumber;
    struct node *linkNode;
    struct edge *nextEdge;
}edge;

node *CreateListAdj();

node *GoToNode(int n, node *node0);

node *GoToEndNode(node *node0);

edge *GoToEndEdge(node *node0);

edge * GoToEdge(int n, node *node0);

void appendNode(char * author,node *node0);

/**Fonction utile pour appendEdge. Evite de des repétitions.
 * prend un sommet et ajout n1 et n2 a la liste de ses aretes.
*/
void appendEdgeSous(edge *edge1,int n1,int n2, node * Node1);
void appendEdgeSousMallock(node *Node1,int n1,int n2, node *node0);
/** Prend le numéros de somet liés et augmente le graphe.*/
void appendEdge(int n1,int n2,node *node0);

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

node* DoListAdjDeBin(options_t *option);

#endif