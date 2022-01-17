#ifndef FONCTIONS_MATRICES_H
#define FONCTIONS_MATRICES_H

#include "analyseGraph.h"
#include "program.h"

/**
 * @brief Generate an unique hash code from the author name
 *
 * @param str
 * @return unsigned
 */
unsigned hash(unsigned char *str, int pr);

node *CreateListAdj();

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
 * @return int
 */
int appendEdgeHash(unsigned int hash1, unsigned int hash2, node **hashTable);

/**
 * @brief Make the adjacence list from the binary file
 *
 * @param option
 * @param size
 * @param hashTable
 * @return node*
 */
node *DoListAdjHash(options_t *option, int *size, node **hashTable);

void freeEdge(node *currentNode);

void freeListAdj(node *node0, int print);

#endif