#ifndef FONCTIONSMATRICE_H
#define FONCTIONSMATRICE_H

#include "program.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure pour faire une liste adjacente.
typedef struct node {
    char author[100];
    int nodeNumber;
    int distance;
    int flag;
    struct edge *nodeEdge;
    struct node *nextNode;
} node;

typedef struct edge {
    struct node *linkNode;
    struct node *otherNode;
    struct edge *nextEdge;
} edge;

/**
 * @brief reset flag and distance of nodes
 *
 * @param node0
 * @return void
 */
void reset(node *node0);

/**
 * @brief takes number of nodes and append the graph
 *
 * @param n1
 * @param n2
 * @param node0
 * @return int
 */
int appendEdge(int n1, int n2, node *node0);

/**
 * @brief print list node and list edge
 *
 * @param node0
 * @return void
 */
void printListAdj(node *node0);

/**
 * @brief print list node of graph
 *
 * @param node0
 * @return void
 */
void printListNode(node *node0);

/**
 * @brief print list edge of graph
 *
 * @param node0
 * @return void
 */
void printListEdge(node *node0);

error_t removeEdgeListe(node *startList, node *nodeEnleve);

/**
 * @brief calculate the distance between 2 authors
 *
 * @param node1
 * @param node2
 * @param size
 * @return error_t
 */
error_t dijkstra(node *node1, node *node2, int size);

/**
 * @brief print nodes that have a distance greater than -1
 *
 * @param options
 * @param node0
 * @return void
 */
void printDistances(options_t *options, node *node0);

/**
 * @brief print the shortest path of authors between 2 node
 *
 * @param Node1
 * @param Node2
 * @param size
 * @return error_t
 */
error_t shortestPath(node *Node1, node *Node2, int size);

/**
 * @brief explore one connected component of the graphe
 *
 * @param node0
 * @return int
 */
int exploreGraph(node *node0);

/**
 * @brief calculate the number of connected component
 *
 * @param node0
 * @return void
 */
void nbrConnectedComponent(node *node0);

node *verifyAuthorHash(options_t *options, node **hashTable, int author0or1);

/**
 * @brief print all the authors at the distance N of the author given
 *
 * @param options
 * @param node0
 * @return error_t
 */
error_t printAuthorAtDist(options_t *options, node *node0);

#endif