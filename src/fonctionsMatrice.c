#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "baseParser.h"
#include "fonctionsMatrice.h"
#include "program.h"
#include "readFunctions.h"

extern int interruptFlag;

node *CreateListAdj(char *author) {
    node *node0 = (node *)malloc(sizeof(node));
    if (node0 == NULL) {
        fprintf(stderr,"creatListAdj:erreur malloc node = NULL");
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

void reinitialise(node *node0) {
    node *currentNode = node0;
    while (currentNode != NULL) {
        currentNode->flag = 0;
        currentNode->distance = 0;
        currentNode = currentNode->nextNode;
    }
}
node *GoToNode(int n, node *node0) {
    node *currentNode = node0;
    for (int k = 0; currentNode->nodeNumber != n; k++) {
        if (currentNode->nextNode == NULL) {
            printf(" dépassement de list:index trop grand ");
            return currentNode;
        }
        currentNode = currentNode->nextNode;
    }
    return currentNode;
}

/* node *GoToEndNode(node *node0) {
    if (node0 == NULL) {
        return NULL;
    }
    node *currentNode = node0;
    while (currentNode->nextNode != NULL) {
        currentNode = currentNode->nextNode;
    }
    return currentNode;
} */

/* edge *GoToEndEdge(node *node0) {
    edge *currentEdge = node0->nodeEdge;
    if (currentEdge == NULL) {
        return NULL;
    }
    while (currentEdge->nextEdge != NULL) {
        currentEdge = currentEdge->nextEdge;
    }
    return currentEdge;
} */

node *appendNode(char *author, node *end) {
    node *newNode = malloc(sizeof(node));
    if (newNode == NULL) {
        fprintf(stderr,"appendNode:erreur malloc node = NULL ");
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

/* Utilisée dans tests.c uniquement */
int appendEdge(int n1, int n2, node *node0) {
    node *Node1 = GoToNode(n1, node0);
    node *Node2 = GoToNode(n2, node0);

    edge *newEdge1 = (edge *)malloc(sizeof(edge));
    if (newEdge1 == NULL) {
        fprintf(stderr,"appendEdge:erreur malloc edge = NULL");
    }

    edge *newEdge2 = (edge *)malloc(sizeof(edge));
    if (newEdge2 == NULL) {
        fprintf(stderr,"appendEdge:erreur malloc edge = NULL");
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

void freeEdge(node *currentNode) {
    edge *currentEdge = currentNode->nodeEdge;
    edge *inter;
    while (currentEdge->nextEdge != NULL) {
        // printf("    cuE:%i\n",currentEdge->otherNode->nodeNumber);
        inter = currentEdge;
        currentEdge = currentEdge->nextEdge;
        free(inter);
    }
    // printf("    cuE:%i\n",currentEdge->otherNode->nodeNumber);
    free(currentEdge);
}
void freeListAdj(node *node0) {
    node *currentNode = node0;
    node *interN;
    while (currentNode->nextNode != NULL) {
        // printf("cuN:%s\n", currentNode->author);
        if (currentNode->nodeEdge != NULL) {
            freeEdge(currentNode);
        }
        interN = currentNode;
        currentNode = currentNode->nextNode;
        free(interN);
    }
    if (currentNode->nodeEdge != NULL) {
        freeEdge(currentNode);
    }
    // printf("cuN:%s\n", currentNode->author);
    free(currentNode);
}

void printListAdj(node *node0) {
    printListNode(node0);
    printListEdge(node0);
}
void printListNode(node *node0) {
    if (node0 == NULL) {
        exit(ERROR_NODE_EQ_NULL);
    }
    node *currentNode = node0;
    printf("\n");
    printf("liste sommet:\n");
    while (currentNode->nextNode != NULL) {
        printf("  %i:%s\n", currentNode->nodeNumber, currentNode->author);
        currentNode = currentNode->nextNode;
    }
    printf("  %i:%s\n", currentNode->nodeNumber, currentNode->author);
}
void printListEdge(node *node0) {
    node *currentNode = node0;
    edge *currentEdge = currentNode->nodeEdge;
    printf("\n");
    printf("liste arretes:\n");
    while (currentNode->nextNode != NULL) {
        currentEdge = currentNode->nodeEdge;
        if (currentEdge != NULL) {
            while (currentEdge->nextEdge != NULL) {
                printf(" %i -> %i|", currentEdge->linkNode->nodeNumber,
                       currentEdge->otherNode->nodeNumber);
                currentEdge = currentEdge->nextEdge;
            }
            printf(" %i -> %i|\n", currentEdge->linkNode->nodeNumber,
                   currentEdge->otherNode->nodeNumber);
        }
        currentNode = currentNode->nextNode;
    }
    while (currentEdge != NULL && currentEdge->nextEdge != NULL) {
        printf(" %i -> %i|", currentEdge->linkNode->nodeNumber,
               currentEdge->otherNode->nodeNumber);
        currentEdge = currentEdge->nextEdge;
    }
    printf("\n");
}

int AuthorInList(char *author, node *node0) {
    node *currentNode = node0;
    while (currentNode->nextNode != NULL) {
        // printf("test présence:%s %s\n",author,currentNode->author);
        if (strcmp(currentNode->author, author) == 0) {
            return currentNode->nodeNumber;
        }
        currentNode = currentNode->nextNode;
    }
    // printf("test présence:%s %s\n",author,currentNode->author);
    if (strcmp(currentNode->author, author) == 0) {
        return currentNode->nodeNumber;
    }
    return -1;
}

int dijkstra(int n1, node *node0, int taille) {
    // une distance de -1 représente une distance infini
    node *node1 = GoToNode(n1, node0);
    node *currentNode = node1;
    edge *currentEdge = currentNode->nodeEdge;
    node *voisin = currentEdge->otherNode;
    // liste des nodes non marqué dont on a changé la distane (!=-1), elle
    // permet de trouver le prochain sommet avec la distance minimum
    node nodeDistance0 = *currentNode;
    nodeDistance0.nextNode = NULL;
    node *ListeDistance = &nodeDistance0;

    currentNode->distance = 0;
    int k = 0;
    while (k < taille) {
        /*printf("\n\nk:%i\n",k);
        printf("tour\n");
        printf("cuurentNode:%s\n",currentNode->author);*/
        currentEdge = currentNode->nodeEdge;
        voisin = currentEdge->otherNode;

        // exploration des voisins de currentNode et mise a jour de leur
        // distance
        while (1) {
            // printf("avant if:%i=>%i\n",voisin->nodeNumber,voisin->distance);
            if (voisin->distance == -1 ||
                voisin->distance > (currentNode->distance + 1)) {
                voisin->distance = currentNode->distance + 1;
                // printf("après
                // if:%i=>%i\n",voisin->nodeNumber,voisin->distance);

                // si voisin non marqué il faut le rajouté dans ListeDistance
                // ou changer sa distance dans la Liste si il y est déjà
                if (voisin->flag == 0) {
                    node newDistance = *voisin;
                    newDistance.nextNode = NULL;
                    if (ListeDistance == NULL) {
                        ListeDistance = &newDistance;
                    } else {
                        // printf("recherche:%s\n",voisin->author);
                        int n = AuthorInList(voisin->author, ListeDistance);
                        if (n == -1) {
                            newDistance.nextNode = ListeDistance;
                            ListeDistance = &newDistance;
                        } else {
                            node *nodeDistance = GoToNode(n, ListeDistance);
                            nodeDistance->distance = voisin->distance;
                        }
                    }
                }
            }

            if (currentEdge->nextEdge == NULL) {
                break;
            }
            currentEdge = currentEdge->nextEdge;
            voisin = currentEdge->otherNode;
        }
        // liste distance=NULL, la distance minimale est l'infinie
        // le graphe n'est pas connexe
        // on arrete djikstra
        if (ListeDistance == NULL) {
            break;
        }

        // cas ListeDistance non NULL donc il existe une distance minimal
        else {
            // recherche du sommet avec la plus petit distance
            node *nodeDistance = ListeDistance;
            node *minNode = nodeDistance;
            int minDistance = nodeDistance->distance;

            while (nodeDistance->nextNode != NULL) {
                if (nodeDistance->distance < minDistance) {
                    minDistance = nodeDistance->distance;
                    minNode = nodeDistance;
                }
                nodeDistance = nodeDistance->nextNode;
            }
            currentNode->flag = 1;
            // on enlève currentNode de la ListeDistance car il est
            // maintenant marqué
            // cas du premier node
            if (currentNode->nodeNumber == ListeDistance->nodeNumber) {
                ListeDistance = ListeDistance->nextNode;
            }
            // cas d'un node au mileu ou à la fin
            else {
                node *currentDistance = ListeDistance;
                node *previousDistance = ListeDistance;
                while (currentDistance->nextNode != NULL &&
                       currentDistance->nodeNumber != currentNode->nodeNumber) {
                    previousDistance = currentDistance;
                    currentDistance = currentDistance->nextNode;
                }
                if (currentDistance->nextNode == NULL) {
                    previousDistance->nextNode = NULL;
                } else {
                    previousDistance->nextNode = currentDistance->nextNode;
                }
            }
            currentNode = minNode;
        }
        k++;
    }
    return OK;
}

void printDistance(int n1, node *node0) {
    node *currentNode = node0;
    while (currentNode!= NULL) {
        if (currentNode->distance != -1) {
            printf("distance entre %i et %i:%i\n",n1,currentNode->nodeNumber,
                   currentNode->distance);
        }
        currentNode = currentNode->nextNode;
    }
}

int plusCourtChemin(int n1, int n2, node *node0, int taille) {
    int exitCode = dijkstra(n1, node0, taille);
    if (exitCode)
        return exitCode;
    printf("plus court chemin entre %i et %i:\n", n2, n1);
    node *currentNode = GoToNode(n2, node0);
    int distance = currentNode->distance;
    if (distance == -1) {
        fprintf(stderr, "pas de chemin entre %i et %i:\n", n2, n1);
        return ERROR_PATH;
    }
    edge *currentEdge = currentNode->nodeEdge;
    node *minVoisin = currentEdge->otherNode;
    while (currentNode->nodeNumber != n1) {
        printf("%s\n", currentNode->author);
        while (currentEdge != NULL) {
            if (currentEdge->otherNode->distance < minVoisin->distance) {
                minVoisin = currentEdge->otherNode;
            }
            currentEdge = currentEdge->nextEdge;
        }
        currentNode = minVoisin;
        currentEdge = currentNode->nodeEdge;
    }
    printf("%s\n", currentNode->author);
    printf("distance entre %i et %i: %i\n", n2, n1, distance);
    return OK;
}

void explorationGraphe(node *node0,int *isolé) {
    node0->flag = 1;
    if(node0->nodeEdge==NULL){
        *isolé+=1;
    }
    edge *currentEdge = node0->nodeEdge;
    while (currentEdge != NULL) {
        node0 = currentEdge->otherNode;
        if(node0->nodeEdge==NULL){
            *isolé+=1;
        }
        if (node0->flag == 0) {
            explorationGraphe(node0,isolé);
        }
        currentEdge = currentEdge->nextEdge;
    }
}
void nbrComposanteConnexe(node *node0) {
    node *currentNode = node0;
    int nbrConnexe = 0;
    int isolé=0;
    while (currentNode != NULL) {   
        if (currentNode->flag == 0) {
            explorationGraphe(currentNode,&isolé);
            nbrConnexe += 1;
        }
        currentNode = currentNode->nextNode;
    }
    printf("nombre de composante connexe:%i\nnombre de sommets isolé parmis les composantes connexes:%i\n"
            ,nbrConnexe,isolé);
}
