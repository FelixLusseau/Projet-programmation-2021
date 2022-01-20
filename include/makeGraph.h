#ifndef FONCTIONS_MATRICES_H
#define FONCTIONS_MATRICES_H

#include "analyseGraph.h"
#include "program.h"

/** Structs for the graph */
typedef struct node {
    char author[85];
    int32_t nodeNumber;
    int16_t distance;
    int8_t flag;
    struct edge *nodeEdge;
    struct node *nextNode;
} node;

typedef struct edge {
    struct node *linkNode;
    struct node *otherNode;
    struct edge *nextEdge;
} edge;

/**
 * @brief Generate an unique hash code from the author name
 *
 * @param str
 * @return unsigned
 */
unsigned hash(unsigned char *str, int pr);

/**
 * @brief create first node of graph
 *
 * @param author
 * @param hashTable
 * @param node0
 */
node *CreateListAdj(char *author, node **hashTabe);

/**
 * @brief Check if the author already is in the graph
 *
 * @param author
 * @param hashTable
 * @return int
 */
int AuthorInListHash(char *author, node **hashTable, int pr);

/**
 * @brief Make the given hash the current node
 *
 * @param hashTable
 * @param hash
 * @return node*
 */
node *GoToNodeHash(node **hashTable, unsigned int hash);

/**
 * @brief Append the list of node of the graphe
 *
 * @param author
 * @param end
 * @return node*
 */
node *appendNode(char *author, node *end);

/**
 * @brief Add an edge to the graph
 *
 * @param hash1
 * @param hash2
 * @param hashTable
 */
void appendEdgeHash(unsigned int hash1, unsigned int hash2, node **hashTable);

/**
 * @brief Make the adjacence list from the binary file
 *
 * @param option
 * @param size
 * @param hashTable
 * @param node0
 * @return error_t
 */
node *doListAdjHash(options_t *option, int *size, node **hashTable);

/**
 * @brief Free edges of a node
 *
 * @param currentNode
 * @return error_t
 */
void freeEdge(node *currentNode);

/**
 * @brief Free graph
 *
 * @param node0
 * @param print
 * @return error_t
 */
void freeListAdj(node *node0, int print);

#endif