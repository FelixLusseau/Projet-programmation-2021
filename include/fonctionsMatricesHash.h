#ifndef FONCTIONS_MATRICES_H
#define FONCTIONS_MATRICES_H

#include "fonctionsMatrice.h"
#include "program.h"

/**
 * @brief Generate an unique hash code from the author name
 *
 * @param str
 * @return unsigned
 */
unsigned hash(unsigned char *str);

/**
 * @brief Check if the author already is in the graph
 *
 * @param author
 * @param hashTable
 * @return int
 */
int AuthorInListHash(char *author, node **hashTable);

/**
 * @brief Make the given hash the current node
 *
 * @param hashTable
 * @param hash
 * @return node*
 */
node *GoToNodeHash(node **hashTable, unsigned int hash);

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
 * @param taille
 * @param hashTable
 * @return node*
 */
node *DoListAdjDeBinHash(options_t *option, int *taille, node **hashTable);

#endif