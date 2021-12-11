#ifndef FONCTIONS_MATRICES_H
#define FONCTIONS_MATRICES_H

#include "fonctionsMatrice.h"
#include "program.h"

unsigned hash(unsigned char *str);

int AuthorInListHash(char *author, node **hashTable);

node *GoToNodeHash(node ** hashTable, unsigned int hash);

/**Fonction utile pour appendEdge. Evite de des repétitions.
 * prend un sommet et ajout n1 et n2 a la liste de ses aretes.
*/
void appendEdgeSousHash(edge *newEdge,int n1,edge *edge0);

/** Prend le numéros de sommets liés et augmente le graphe.*/
int appendEdgeHash(int n1, unsigned int hash1, int n2, unsigned int hash2, node *node0, node ** hashTable);

node *DoListAdjDeBinHash(options_t *option, int *taille);

#endif