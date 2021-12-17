#ifndef FONCTIONS_MATRICES_H
#define FONCTIONS_MATRICES_H

#include "fonctionsMatrice.h"
#include "program.h"

unsigned hash(unsigned char *str);

int AuthorInListHash(char *author, node **hashTable);

node *GoToNodeHash(node ** hashTable, unsigned int hash);

/** Prend le numéros de sommets liés et augmente le graphe.*/
int appendEdgeHash( unsigned int hash1, unsigned int hash2,  node ** hashTable);

node *DoListAdjDeBinHash(options_t *option, int *taille);

#endif