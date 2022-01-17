#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "analyseGraph.h"
#include "baseParser.h"
#include "makeGraph.h"

#include "program.h"
#include "readFunctions.h"
#include "tps_unit_test.h"

extern int interruptFlag;

/* void reinitialise(node *node0) {
    node *currentNode = node0;
    while (1) {
        printf("reinitialise:%i--%s\n", currentNode->nodeNumber,
               currentNode->author);
        currentNode->flag = 0;
        currentNode->distance = -1;
        if (currentNode->nextNode == NULL) {
            break;
        }
        currentNode = currentNode->nextNode;
    }
} */

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
        if (interruptFlag == 1) {
            break;
        }
        printf("  %i:%s\n", currentNode->nodeNumber, currentNode->author);
        currentNode = currentNode->nextNode;
    }
    printf("  %i:%s\n", currentNode->nodeNumber, currentNode->author);
}

void sousprintListEdge(node *node0) {
    edge *currentEdge = node0->nodeEdge;
    printf(" %s -> %s|", currentEdge->linkNode->author,
           currentEdge->otherNode->author);
    while (currentEdge->nextEdge != NULL) {
        printf(" %s -> %s|", currentEdge->linkNode->author,
               currentEdge->otherNode->author);
        currentEdge = currentEdge->nextEdge;
    }
    printf(" %s -> %s|", currentEdge->linkNode->author,
           currentEdge->otherNode->author);
}

void printListEdge(node *node0) {
    node *currentNode = node0;
    edge *currentEdge = currentNode->nodeEdge;
    printf("\n");
    printf("liste arretes:\n");
    while (currentNode->nextNode != NULL) {
        if (interruptFlag == 1) {
            break;
        }
        currentEdge = currentNode->nodeEdge;
        if (currentEdge != NULL) {
            sousprintListEdge(currentNode);
        }
        currentNode = currentNode->nextNode;
    }
    if (currentEdge != NULL) {
        sousprintListEdge(currentNode);
    }
    printf("\n");
}

node *popListeEdge(node *debutListe) {
    node *node0 = debutListe->nodeEdge->linkNode;
    edge *inter = debutListe->nodeEdge;
    debutListe->nodeEdge = debutListe->nodeEdge->nextEdge;
    free(inter);
    return node0;
}
error_t enleveEdgeListe(node *debutListe, node *nodeEnleve) {
    if (debutListe->nodeEdge->linkNode->nodeNumber == nodeEnleve->nodeNumber) {
        edge *inter = debutListe->nodeEdge;
        debutListe->nodeEdge = debutListe->nodeEdge->nextEdge;
        free(inter);
        return ERROR_DIJKSTRA;
    }
    edge *currentEdge = debutListe->nodeEdge;
    edge *previousEdge = debutListe->nodeEdge;
    while (currentEdge->linkNode->nodeNumber != nodeEnleve->nodeNumber) {
        previousEdge = currentEdge;
        currentEdge = currentEdge->nextEdge;
    }
    previousEdge->nextEdge = currentEdge->nextEdge;
    free(currentEdge);
    return OK;
}
edge *appendListeEdge(edge *endListe, node *newNode, node *debutListe) {
    edge *newListe = (edge *)malloc(sizeof(edge));
    if (newListe == NULL) {
        fprintf(stderr, "appendListeEdge:erreur malloc node = NULL ");
        exit(1);
    }
    newListe->linkNode = newNode;
    newListe->nextEdge = NULL;

    if (debutListe->nodeEdge == NULL) {
        debutListe->nodeEdge = newListe;
        return newListe;
    }
    endListe->nextEdge = newListe;
    return newListe;
}

error_t dijkstra(node *node1, node *node2, int taille) {
    printf("*************************************Debut dijkstra"
           "*************************************\n\n");
    // flag pour determiner si on cherche une distance entre 2 noude ou tout la
    // composante connexe
    int flagGraphe = 0;
    if (node2 == NULL) {
        flagGraphe = 1;
    }
    // une distance de -1 représente une distance infini
    node *currentNode = node1;
    currentNode->distance = 0;
    edge *currentEdge = currentNode->nodeEdge;
    node *voisin = currentEdge->otherNode;
    int flag = voisin->distance;

    /* liste des nodes non marqués dont on a changé la distane (!=-1), elle
    permet de trouver le prochain sommet avec la distance minimum*/
    node *ListeDistance = CreateListAdj("0");
    if (ListeDistance == NULL)
        return ERROR_NODE_EQ_NULL;
    ListeDistance->distance = 1;
    edge *endListe = appendListeEdge(NULL, currentNode, ListeDistance);
    int k = 0;

    while (k < taille) {
        // printf("k:%i/%i--ListeD:%i
        // NN:%s\n",k,2664528,ListeDistance->distance,currentNode->author);
        currentEdge = currentNode->nodeEdge;
        voisin = currentEdge->otherNode;
        /* exploration des voisins non marqué de currentNode
        et mise a jour de leur distance*/
        while (currentEdge != NULL) {
            flag = voisin->distance;
            if (voisin->distance == -1 ||
                (voisin->distance > (currentNode->distance + 1) &&
                 voisin->flag == 0)) {

                voisin->distance = currentNode->distance + 1;
                /* si le voisin avait une distance -1 précedemment
                il n'était pas dans la Liste, on le rajoute*/
                if (flag == -1) {
                    endListe = appendListeEdge(endListe, voisin, ListeDistance);
                    ListeDistance->distance += 1;
                }
            }
            if (currentEdge->nextEdge == NULL) {
                break;
            }
            currentEdge = currentEdge->nextEdge;
            voisin = currentEdge->otherNode;
        }

        currentNode->flag = 1;
        // on enlève currentNode de la ListeDistance car il est maintenant
        // marqué
        enleveEdgeListe(ListeDistance, currentNode);
        ListeDistance->distance -= 1;

        /*liste distance=NULL, la distance minimale est l'infinie:
        le graphe n'est pas connexe,on arrete dijkstra*/
        if (ListeDistance->nodeEdge == NULL) {
            printf("distance NULL\n");
            break;
        }
        if (flagGraphe == 0 && node2->flag == 1) {
            break;
        }

        // cas ListeDistance non NULL donc il existe une distance minimal
        else {
            // recherche du sommet avec la plus petit distance
            edge *ListeEdge = ListeDistance->nodeEdge;
            node *minNode = ListeEdge->linkNode;
            int minDistance = minNode->distance;

            while (1) {
                // si on trouve une distance = à celle du currentNode, c'est un
                // minimum
                if (ListeEdge->linkNode->distance == currentNode->distance) {
                    minNode = ListeEdge->linkNode;
                    break;
                }

                if (ListeEdge->linkNode->distance < minDistance) {
                    minNode = ListeEdge->linkNode;
                    minDistance = minNode->distance;
                }
                if (ListeEdge->nextEdge == NULL) {
                    break;
                }
                ListeEdge = ListeEdge->nextEdge;
            }
            currentNode = minNode;
        }
        k++;
    }
    if (flagGraphe == 0) {
        printf("Distance: %i\n", node2->distance);
    }

    freeListAdj(ListeDistance, 0);
    printf("\n*************************************Fin dijkstra"
           "*************************************\n\n");
    return OK;
}

void printDistances(options_t *options, node *node0) {
    node *currentNode = node0;
    printf("Distances : \n");
    while (currentNode != NULL) {
        if (currentNode->distance != -1) {
            printf("%s - %s : %i\n", options->authorNames[0],
                   currentNode->author, currentNode->distance);
        }
        currentNode = currentNode->nextNode;
    }
}

error_t plusCourtChemin(node *Node1, node *Node2, int taille) {
    int exitCode = dijkstra(Node1, Node2, taille);
    if (exitCode)
        return exitCode;
    int len;

    printf("\nPlus court chemin entre %s et %s :\n\n", Node1->author,
           Node2->author);
    node *currentNode = Node2;
    int distance = currentNode->distance;
    if (distance == -1) {
        fprintf(stderr, "Pas de chemin entre %s et %s\n", Node1->author,
                Node2->author);
        return ERROR_PATH;
    }

    edge *currentEdge = currentNode->nodeEdge;
    node *minVoisin = currentEdge->otherNode;
    while (currentNode->nodeNumber != Node1->nodeNumber) {
        len = (50 - strlen(currentNode->author)) / 2;
        printf("%*s%s%*s\n", len, "", currentNode->author, len, "");
        printf("%*s\33[0;32m|\33[0m%*s\n", 25, "", 25, "");
        while (currentEdge != NULL) {
            if (currentEdge->linkNode->flag == 1 &&
                currentEdge->otherNode->distance < minVoisin->distance) {
                minVoisin = currentEdge->otherNode;
            }
            currentEdge = currentEdge->nextEdge;
        }
        currentNode = minVoisin;
        currentEdge = currentNode->nodeEdge;
    }
    len = (50 - strlen(currentNode->author)) / 2;
    printf("%*s%s%*s\n", len, "", currentNode->author, len, "");
    printf("\nDistance entre %s et %s : %i\n", Node1->author, Node2->author,
           distance);
    return OK;
}

int explorationGraphe(node *node0) {
    int nbrNode = 0;

    node0->flag = 1;
    if (node0->nodeEdge == NULL) {
        return 0;
    }
    edge *currentEdge = node0->nodeEdge;
    node *voisin = currentEdge->otherNode;

    node *debutListe = CreateListAdj("0");
    edge *endListe = appendListeEdge(NULL, node0, debutListe);
    while (debutListe->nodeEdge != NULL) {
        nbrNode += 1;
        node0 = popListeEdge(debutListe);
        currentEdge = node0->nodeEdge;
        voisin = currentEdge->otherNode;

        while (1) {
            if (voisin->flag == 0) {
                voisin->flag = 1;
                endListe = appendListeEdge(endListe, voisin, debutListe);
            }
            if (currentEdge->nextEdge == NULL) {
                break;
            }
            currentEdge = currentEdge->nextEdge;
            voisin = currentEdge->otherNode;
        }
    }
    free(debutListe);
    return nbrNode;
}

void nbrComposanteConnexe(node *node0) {
    node *currentNode = node0;
    int nbrConnexe = 0;
    int isole = 0;
    while (currentNode != NULL) {
        if (interruptFlag == 1) {
            break;
        }
        if (currentNode->flag == 0) {
            explorationGraphe(currentNode);
            nbrConnexe += 1;
        }
        currentNode = currentNode->nextNode;
    }
    printf("Nombre de composantes connexes : %i\nDont sommets isolés : %i\n",
           nbrConnexe, isole);
}

node *verifyAuthorHash(options_t *options, node **hashTable, int author0or1) {
    int pr[4] = {pr1, pr2, pr3, pr4};
    for (int p = 0; p < 4; p++) {
        if (strcmp(hashTable[hash((unsigned char *)
                                      options->authorNames[author0or1],
                                  pr[p])]
                       ->author,
                   options->authorNames[author0or1]) == 0) {
            return hashTable[hash(
                (unsigned char *)options->authorNames[author0or1], pr[p])];
        }
    }
    return NULL;
}

error_t printAuthorAtDist(options_t *options, node *node0) {
    if (node0 == NULL) {
        return ERROR_NODE_EQ_NULL;
    }
    node *currentNode = node0;
    printf("\nAuthors at distance %i of %s : \n", options->N,
           options->authorNames[0]);
    while (currentNode->nextNode != NULL) {
        if (currentNode->distance == options->N) {
            printf(" - %s\n", currentNode->author);
        }
        currentNode = currentNode->nextNode;
        if (interruptFlag == 1)
            break;
    }
    return OK;
}