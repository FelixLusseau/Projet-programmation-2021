#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "program.h"
#include "fonctionsMatrice.h"
#include "baseParser.h"
#include "readBinary.h"

extern int interruptFlag;

node *CreateListAdj(char *author){
    node *node0 = (node *)malloc(sizeof(node));
    if(node0==NULL){
        printf("creatListAdj:erreur malloc node = NULL");
        return NULL;
    }
    int k=0;
    while(author[k]!='\0'){
        node0->author[k]=author[k];
        k++;
    }
    node0->author[k]='\0';
    node0->nodeNumber=0;
    node0->flag=0;
    node0->nextNode=NULL;
    node0->distance=-1;

    edge *edge0=malloc(sizeof(edge));
    edge0->edgeNumber=0;
    edge0->otherNode=NULL;
    edge0->linkNode=node0;
    edge0->nextEdge=NULL;

    node0->nodeEdge=edge0;

    return node0;
}
node *GoToNode(int n, node *node0)
{
    node *currentNode = node0;
    for (int k = 0; currentNode->nodeNumber < n; k++)
    {
        if(currentNode->nextNode==NULL){
            printf(" dépassement de list:index trop grand ");
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

node *appendNode(char * author,node *end){
    node *newNode=malloc(sizeof(node));
    if(newNode==NULL){
        printf(" appendNode:erreur malloc node = NULL ");
    }
    node *currentNode = end;
    currentNode->nextNode=newNode;
    newNode->nextNode=NULL;
    newNode->nodeNumber=currentNode->nodeNumber+1;
    newNode->distance=-1;
    newNode->flag=0;
    newNode->nodeEdge=NULL;
    int k=0;
    while(author[k]!='\0'){
        newNode->author[k]=author[k];
        k++;
    }
    newNode->author[k]='\0';
    return newNode;
}

void appendEdgeSous(edge *newEdge,int n1,edge *edge0){
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
int appendEdge(int n1,int n2,node *node0)
{   edge *edge0=node0->nodeEdge;
    node *Node1=GoToNode(n1,node0);
    node *Node2=GoToNode(n2,node0);
    //cas de la 1ére arrête deu nodeO
    if((n1==0 || n2==0) && edge0->otherNode==NULL){
        if(n1==0){
            edge0->otherNode=Node2;
            edge *newEdge2 = (edge *)malloc(sizeof(edge));
            if(newEdge2==NULL){
                printf("appendEdge:erreur malloc edge = NULL");
            }
            newEdge2->otherNode=Node1;
            newEdge2->linkNode=Node2;
            appendEdgeSous(newEdge2,n2,edge0);
        }
        else{
            edge0->otherNode=Node1;
            edge *newEdge1 = (edge *)malloc(sizeof(edge));
            if(newEdge1==NULL){
                printf("appendEdge:erreur malloc edge = NULL");
            }
            newEdge1->otherNode=Node2;
            newEdge1->linkNode=Node1;
            appendEdgeSous(newEdge1,n1,edge0);
        }
        return 0;
    }
    //initalisation des nouveau edges
    edge *newEdge1 = (edge *)malloc(sizeof(edge));
     if(newEdge1==NULL){
        printf("appendEdge:erreur malloc edge = NULL");
    }
    newEdge1->otherNode=Node2;
    newEdge1->linkNode=Node1;
    newEdge1->edgeNumber=0;

    edge *newEdge2 = (edge *)malloc(sizeof(edge));
     if(newEdge2==NULL){
        printf("appendEdge:erreur malloc edge = NULL");
    }
    newEdge2->otherNode=Node1;
    newEdge2->linkNode=Node2;
    newEdge2->edgeNumber=0;

    if(Node1->nodeEdge==NULL){
        Node1->nodeEdge=newEdge1;
    }
    appendEdgeSous(newEdge1,n1,edge0);

    if(Node2->nodeEdge==NULL){
        Node2->nodeEdge=newEdge2;
    }
    appendEdgeSous(newEdge2,n2,edge0);
    return 0;
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
    printListNode(node0);
    printListEdge(node0);
}
void printListNode(node * node0){
    if(node0==NULL){
        exit(0);
    }
    node *currentNode=node0;
    printf("\n");
    printf("liste sommet:\n");
    while(currentNode->nextNode !=NULL){
        printf("  %i:%s->%i |",currentNode->nodeNumber,currentNode->author,currentNode->nodeEdge->edgeNumber);
        //printf("  %i |",currentNode->indexEdge);
        currentNode=currentNode->nextNode;
    }
    printf("  %i:%s->%i |",currentNode->nodeNumber,currentNode->author,currentNode->nodeEdge->edgeNumber);
    //printf("  %i |\n",currentNode->indexEdge);
}
void printListEdge(node * node0){
    edge *currentEdge=node0->nodeEdge;
    printf("\n");
    printf("liste arretes:\n");
    while(currentEdge->nextEdge !=NULL){
        printf(" %i :%i -> %i|",currentEdge->edgeNumber, currentEdge->otherNode->nodeNumber,currentEdge->linkNode->nodeNumber);
        //printf(" %i |",currentEdge->otherNode->nodeNumber);
        currentEdge=currentEdge->nextEdge;
    }
    printf(" %i :%i -> %i|\n",currentEdge->edgeNumber,currentEdge->otherNode->nodeNumber,currentEdge->linkNode->nodeNumber);
    //printf(" %i |\n",currentEdge->otherNode->nodeNumber);
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


node* DoListAdjDeBin(options_t *option,int *taille){
    printf("************Debut DoListAdjDeBin************\n");
    *taille=0;
    int curseur=1;
    structureBase_t Entree;
    initStructure(&Entree, 0);
    Entree = readEntryBin(option, -1);
    curseur++;
    
    if(Entree.author[0]==NULL){
        printf("Erreur 1er livre author[0]=NULL");
        return NULL;
    }

    node *node0=CreateListAdj(Entree.author[0]);
    int n1=0;
    int n2=0;
    node *end=node0;

    for(int k=1; k<Entree.authornb;k++){
        char *author1=Entree.author[k];
        n1 = AuthorInList(author1,node0);
        if(n1<0){
            end=appendNode(author1,end);
            *taille+=1;
            n1=*taille;
        }
        for(int i=k+1; i<Entree.authornb;i++){
            char *author2=Entree.author[i];
            n2 = AuthorInList(author2,node0);
            if(n2<0){
                end=appendNode(author2,end);
                *taille+=1;
                n2=*taille;
            }
            appendEdge(n1,n2,node0);
        }
    }
    
    int L[100];
    while(Entree.authornb!=0){
        //printf("curseur:%i ",curseur);
        if(interruptFlag==1){
            break;
        }
        if(Entree.authornb>1){
            L[0]=-1;
            int index=0;
            for(int k=0; k<=Entree.authornb;k++){
                char *author1=Entree.author[k];
                if(strcmp(author1,"")!=0){
                    n1 = AuthorInList(author1,node0);
                    if(n1<0){
                        end=appendNode(author1,end);
                        *taille+=1;
                        n1=*taille; 
                    }
                    L[index]=n1;
                    index++;
                    L[index]=-1; 
                }
            }
            for(int i=0;L[i]>-1 && i<100;i++){
                for(int k=i+1;L[k]>-1 && k<100;k++){
                    appendEdge(L[i],L[k],node0);
                }
            }
        }
        Entree = readEntryBin(option, -1);
        curseur++;
    }
    printf("************Fin DoListAdjDeBin************\n");
    return node0;
}


void Dijkstra(int n1,node *node0,int taille){
    node *node1=GoToNode(n1,node0);
    node *currentNode=node1;
    edge *currentEdge=currentNode->nodeEdge;
    node *voisin=currentEdge->otherNode;

    currentNode->distance=0;
    int k=0;
    while(k<taille && currentNode->distance>=0){
        currentEdge=currentNode->nodeEdge;
        voisin=GoToNode(currentEdge->otherNode->nodeNumber,node0);

        //exploration des voisins de currentNode et mise a jour de leur distance
        while(1){
            if(voisin->flag==0){
                if(voisin->distance==-1 || voisin->distance > (currentNode->distance +1)){
                    voisin->distance=currentNode->distance +1;
                }
            }

            if(currentEdge->nextEdge==NULL){
                break;
            }

            currentEdge=currentEdge->nextEdge;
            if(currentEdge->linkNode->nodeNumber!=currentNode->nodeNumber){
                break;
            }
            voisin=GoToNode(currentEdge->otherNode->nodeNumber,node0);
        }

        currentEdge=currentNode->nodeEdge;
        voisin=currentEdge->otherNode;
        node *minVoisin=voisin;
        int mindistance=minVoisin->distance;
        //recherche du voisin avec la plus petit distance
        while(1){
            if(voisin->flag==0 && voisin->distance>0 && voisin->distance < mindistance ){
                minVoisin=voisin;
                mindistance=minVoisin->distance;
            }

            if(currentEdge->nextEdge==NULL){
                break;
            }

            currentEdge=currentEdge->nextEdge;

            if(currentEdge->linkNode->nodeNumber!=currentNode->nodeNumber){
                break;
            }

            voisin=currentEdge->otherNode;
        }

        k++;
        currentNode->flag=1;
        currentNode=minVoisin;
    }
}

void printDistance(int n1,node *node0){
    node *currentNode=node0;
    while(currentNode->nextNode!=NULL){
        printf("author:%s distance de %i:%i\n",currentNode->author,n1,currentNode->distance);
        currentNode=currentNode->nextNode;
    }
    printf("author:%s distance de %i:%i\n",currentNode->author,n1,currentNode->distance);
}
