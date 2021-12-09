#ifndef FONCTIONS_MATRICES_H
#define FONCTIONS_MATRICES_H

#include "fonctionsMatrice.h"
#include "program.h"

unsigned hash(unsigned char *str);

int AuthorInListHash(char *author, node **hashTable);

node *DoListAdjDeBinHash(options_t *option, int *taille);

#endif