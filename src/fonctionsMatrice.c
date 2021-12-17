#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "baseParser.h"
#include "fonctionsMatrice.h"
#include "program.h"
#include "readBinary.h"

extern int interruptFlag;

node *CreateListAdj(char *author) {
    node *node0 = (node *)malloc(sizeof(node));
    if (node0 == NULL) {
        printf("creatListAdj:erreur malloc node = NULL");
        return NULL;
    }
    int k = 0;
    while (author[k] != '\0') {
        node0->author[k] = author[k];
        k++;
    }
    node0->author[k] = '\0';
    node0->nodeNumber = 0;
    node0->flag = 0;
    node0->nextNode = NULL;
    node0->distance = -1;
    node0->nodeEdge = NULL;

    return node0;
}
node *GoToNode(int n, node *node0) {
    node *currentNode = node0;
    for (int k = 0; currentNode->nodeNumber < n; k++) {
        if (currentNode->nextNode == NULL) {
            printf(" dépassement de list:index trop grand ");
            return currentNode;
        }
        currentNode = currentNode->nextNode;
    }
    return currentNode;
}
node *GoToEndNode(node *node0) {
    if (node0 == NULL) {
        return NULL;
    }
    node *currentNode = node0;
    while (currentNode->nextNode != NULL) {
        currentNode = currentNode->nextNode;
    }
    return currentNode;
}

edge *GoToEndEdge(node *node0) {
    edge *currentEdge = node0->nodeEdge;
    if (currentEdge == NULL) {
        return NULL;
    }
    while (currentEdge->nextEdge != NULL) {
        currentEdge = currentEdge->nextEdge;
    }
    return currentEdge;
}

node *appendNode(char *author, node *end) {
    node *newNode = malloc(sizeof(node));
    if (newNode == NULL) {
        printf(" appendNode:erreur malloc node = NULL ");
    }
    node *currentNode = end;
    currentNode->nextNode = newNode;
    newNode->nextNode = NULL;
    newNode->nodeNumber = currentNode->nodeNumber + 1;
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
int appendEdge(int n1, int n2, node *node0) {
    node *Node1 = GoToNode(n1, node0);
    node *Node2 = GoToNode(n2, node0);

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
    newEdge1->nextEdge = Node1->nodeEdge->nextEdge;
    Node1->nodeEdge->nextEdge = newEdge1;

    newEdge2->otherNode = Node1;
    newEdge2->linkNode = Node2;
    newEdge2->nextEdge = Node2->nodeEdge->nextEdge;
    Node2->nodeEdge->nextEdge = newEdge2;

    return 0;
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
void freeListAdj(node *node0) {
    node *currentNode = node0;
    node *interN;
    while (currentNode->nextNode != NULL) {
        freeEdge(currentNode);
        interN = currentNode;
        currentNode = currentNode->nextNode;
        free(interN);
    }
    free(currentNode);
}

void printListAdj(node *node0) {
    printListNode(node0);
    printListEdge(node0);
}
void printListNode(node *node0) {
    if (node0 == NULL) {
        exit(0);
    }
    node *currentNode = node0;
    printf("\n");
    printf("liste sommet:\n");
    while (currentNode->nextNode != NULL) {
        printf("  %s:%i |", currentNode->author, currentNode->nodeNumber);
        currentNode = currentNode->nextNode;
    }
    printf("  %s:%i |", currentNode->author, currentNode->nodeNumber);
}
void printListEdge(node *node0) {
    edge *currentEdge = node0->nodeEdge;
    printf("\n");
    printf("liste arretes:\n");
    if (currentEdge->otherNode == NULL) {
        printf(" %i -> persone|", currentEdge->linkNode->nodeNumber);
        currentEdge = currentEdge->nextEdge;
    }
    while (currentEdge->nextEdge != NULL) {
        printf(" %i -> %i|", currentEdge->linkNode->nodeNumber,
               currentEdge->otherNode->nodeNumber);
        // printf(" %i |",currentEdge->otherNode->nodeNumber);
        currentEdge = currentEdge->nextEdge;
    }
    printf(" %i -> %i|\n", currentEdge->linkNode->nodeNumber,
           currentEdge->otherNode->nodeNumber);
    // printf(" %i |\n",currentEdge->otherNode->nodeNumber);
}

int AuthorInList(char *author, node *node0) {
    node *currentNode = node0;
    while (currentNode->nextNode != NULL) {
        if (strcmp(currentNode->author, author) == 0) {
            return currentNode->nodeNumber;
        }
        currentNode = currentNode->nextNode;
    }
    if (strcmp(currentNode->author, author) == 0) {
        return currentNode->nodeNumber;
    }
    return -1;
}

node *DoListAdjDeBin(options_t *option, int *taille) {
    int nbrarrete = 0;
    printf("************Debut DoListAdjDeBin************\n");
    *taille = 0;
    int curseur = 1;
    structureBase_t Entree;
    initStructure(&Entree, 0);
    Entree = readEntryBin(option, -1);
    curseur++;

    if (Entree.author[0] == NULL) {
        printf("Erreur 1er livre author[0]=NULL");
        return NULL;
    }

    node *node0 = CreateListAdj(Entree.author[0]);
    int n1 = 0;
    int n2 = 0;
    node *end = node0;

    for (int k = 1; k < Entree.authornb; k++) {
        char *author1 = Entree.author[k];
        n1 = AuthorInList(author1, node0);
        // printf("n1 : %i\n", n1);
        if (n1 < 0) {
            end = appendNode(author1, end);
            *taille += 1;
            n1 = *taille;
        }
        for (int i = k + 1; i < Entree.authornb; i++) {
            char *author2 = Entree.author[i];
            n2 = AuthorInList(author2, node0);
            printf("n2 : %i\n", n2);
            if (n2 < 0) {
                end = appendNode(author2, end);
                *taille += 1;
                n2 = *taille;
            }
            appendEdge(n1, n2, node0);
        }
    }
    // printf("OK");
    int L[100];
    while (Entree.authornb != 0) {
        printf("\rcurseur:%i ", curseur);
        if (interruptFlag == 1) {
            break;
        }
        if (Entree.authornb > 1) {
            L[0] = -1;
            int index = 0;
            for (int k = 0; k < Entree.authornb; k++) {
                char *author1 = Entree.author[k];
                n1 = AuthorInList(author1, node0);
                // printf("n1 : %i\n", n1);
                if (n1 < 0) {
                    end = appendNode(author1, end);
                    *taille += 1;
                    n1 = *taille;
                    // printf("n1 = *taille : %i\n", n1);
                }
                L[index] = n1;
                index++;
                L[index] = -1;
            }
            for (int i = 0; L[i] > -1 && i < 100; i++) {
                for (int k = i + 1; L[k] > -1 && k < 100; k++) {
                    appendEdge(L[i], L[k], node0);
                    nbrarrete++;
                    // printf("Li : %i Lk : %i\n", L[i], L[k]);
                }
            }
        }
        Entree = readEntryBin(option, -1);
        curseur++;
    }
    printf("normal nbr arret:%i\n", nbrarrete * 2);
    printf("************Fin DoListAdjDeBin************\n");
    return node0;
}

/* void Dijkstra(int n1,node *node0,int taille){
    // une distance de -1 représente une distance infini
    node *node1=GoToNode(n1,node0);
    node *currentNode=node1;
    edge *currentEdge=currentNode->nodeEdge;
    node *voisin=currentEdge->otherNode;
    //liste des nodes non marqué dont on a changé la distane (!=-1), elle permet
    //de trouver le prochain sommet avec la distance minimum
    node nodeDistance0=*currentNode;
    nodeDistance0.nextNode=NULL;
    node *ListeDistance=&nodeDistance0;

    currentNode->distance=0;
    int k=0;
    while(k<taille && currentNode->distance>=0){
        currentEdge=currentNode->nodeEdge;
        voisin=GoToNode(currentEdge->otherNode->nodeNumber,node0);

        //exploration des voisins de currentNode et mise a jour de leur distance
        while(currentEdge->linkNode->nodeNumber==currentNode->nodeNumber){

            if(voisin->flag==0){
                if(voisin->distance==-1 || voisin->distance >
(currentNode->distance +1)){ voisin->distance=currentNode->distance +1;

                    if(ListeDistance==NULL){
                        node newDistance0=*voisin;
                        newDistance0.nextNode=NULL;
                        ListeDistance=&newDistance0;
                    }
                    int n=AuthorInList(voisin->author,ListeDistance);
                    if(n==-1){
                        node newDistance =*voisin;
                        newDistance.nextNode=ListeDistance->nextNode;
                        ListeDistance->nextNode=&newDistance;
                    }
                    else{
                        node *nodeDistance=GoToNode(n,ListeDistance);
                        nodeDistance->distance=voisin->distance;
                    }
                }
            }

            if(currentEdge->nextEdge==NULL){
                break;
            }
            currentEdge=currentEdge->nextEdge;
            voisin=currentEdge->otherNode;
        }
        //cas ListeDistance non NULL donc il existe une distance minimal
        if(ListeDistance!=NULL){
            //recherche du sommet avec la plus petit distance
            node *nodeDistance=ListeDistance;
            node *minNode=nodeDistance;
            int minDistance=nodeDistance->distance;

            while(nodeDistance->nextNode!=NULL){
                if(nodeDistance->distance<minDistance){
                    minDistance=nodeDistance->distance;
                    minNode=nodeDistance;
                }
                nodeDistance=nodeDistance->nextNode;
            }
            currentNode->flag=1;
            //on enlève currentNode de la liste nodeDistance0 car il est
maintenant marqué
            //cas du premier node
            if(currentNode->nodeNumber==ListeDistance->nodeNumber){
                ListeDistance=ListeDistance->nextNode;
            }
            //cas d'un node au mileu ou à la fin
            else{
                node *currentDistance=ListeDistance;
                node *previousNode=ListeDistance;
                while(currentDistance->nodeNumber!=currentNode->nodeNumber &&
currentNode->nextNode!=NULL){ previousNode=currentNode;
                }
                previousNode->nextNode=currentNode->nextNode;
            }
            currentNode=minNode;
        }
        //liste distance NULL, la distance minimale est l'infinie
        // on prend le 1er Node non marqué de distance infinie
        else{
            currentNode=node0;
            while(currentNode->flag!=0 && currentNode->distance!=-1){
                currentNode=currentNode->nextNode;
            }
        }
        k++;
    }
}

void printDistance(int n1,node *node0){
    node *currentNode=node0;
    while(currentNode->nextNode!=NULL){
        printf("author:%s distance de
%i:%i\n",currentNode->author,n1,currentNode->distance);
        currentNode=currentNode->nextNode;
    }
    printf("author:%s distance de
%i:%i\n",currentNode->author,n1,currentNode->distance);
}*/
