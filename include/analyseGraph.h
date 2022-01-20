#ifndef FONCTIONSMATRICE_H
#define FONCTIONSMATRICE_H

#include "makeGraph.h"
#include "program.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

/**
 * @brief Used in dijkstra and exploreConnectedComponent to keep track of the
 * node that they use. Créer la liste.
 *
 * @param void
 * @return node
 */
node *createList(void);
/**
 * @brief Used in dijkstra and exploreConnectedComponent to keep track of the
 * node that they use. Pop first node of the list.
 *
 * @param startList
 * @return node
 */
node *popListeEdge(node *startList);
/**
 * @brief Used in dijkstra and exploreConnectedComponent to keep track of the
 * node that they use. Find end remove a node of the list.
 *
 * @param startList
 * @param nodeToRemove
 * @return node
 */
error_t removeEdgeListe(node *startList, node *nodeToRemove);
/**
 * @brief Used in dijkstra and exploreConnectedComponent to keep track of the
 * node that they use. Append end of the list
 *
 * @param endList
 * @param newNode
 * @param startList
 * @return edge
 */
edge *appendListeEdge(edge *endListe, node *newNode, node *startList);

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

/**
 * @brief receives an author name and responds the pointer to this author after
 * having verified that the hash calculated isn't an other's due to a collision
 *
 * @param options
 * @param hashTable
 * @param author0or1
 * @return node*
 */
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