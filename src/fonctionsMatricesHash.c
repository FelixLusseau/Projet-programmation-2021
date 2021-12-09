#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "program.h"
#include "fonctionsMatrice.h"
#include "baseParser.h"
#include "readBinary.h"

extern int interruptFlag;

unsigned hash(unsigned char *str)
{
    unsigned hash = 5381;
    int c;
    while ((c = *str++))
        hash = ((hash << 5) + hash) + c;
    hash %= 50000000;
    //printf("%u\n", hash);
    return hash;
}

int AuthorInListHash(char *author, node **hashTable)
{
    unsigned int authorHashed = hash((unsigned char *)author);
    if (hashTable[authorHashed] == NULL)
        return -1;
    else
        return hashTable[authorHashed]->nodeNumber;
}

node *DoListAdjDeBinHash(options_t *option, int *taille)
{
    printf("************Debut DoListAdjDeBinHash************\n");
    node **hashTable = malloc(50000000 * sizeof(unsigned int) * sizeof(char *));
    if (hashTable == NULL)
        return NULL;
    for (int i = 0; i < 50000000; i++)
        hashTable[i] = NULL;
    *taille = 0;
    structureBase_t Entree;
    initStructure(&Entree, 0);
    Entree = readEntryBin(option, -1);

    if (Entree.author[0] == NULL)
    {
        printf("Erreur 1er livre author[0]=NULL");
        return NULL;
    }
    node *node0 = CreateListAdj(Entree.author[0]);
    int n1 = 0;
    int n2 = 0;
    node *end = node0;
    for (int k = 1; k < Entree.authornb; k++)
    {
        char *author1 = Entree.author[k];
        n1 = AuthorInListHash(author1, hashTable);
        if (n1 < 0)
        {
            end = appendNode(author1, end);
            hashTable[hash((unsigned char *)author1)]=end;
            *taille += 1;
            n1 = *taille;
        }
        for (int i = k + 1; i < Entree.authornb; i++)
        {
            char *author2 = Entree.author[i];
            n2 = AuthorInListHash(author2, hashTable);
            if (n2 < 0)
            {
                end = appendNode(author2, end);
                hashTable[hash((unsigned char *)author2)]=end;
                *taille += 1;
                n2 = *taille;
            }
            appendEdge(n1, n2, node0);
        }
    }
    int L[100];
    while (Entree.authornb != 0)
    {
        if (interruptFlag == 1)
        {
            break;
        }
        if (Entree.authornb > 1)
        {
            L[0] = -1;
            int index = 0;
            for (int k = 0; k <= Entree.authornb; k++)
            {
                char *author1 = Entree.author[k];
                if (strcmp(author1, "") != 0)
                {
                    n1 = AuthorInListHash(author1, hashTable);
                    if (n1 < 0)
                    {
                        end = appendNode(author1, end);
                        hashTable[hash((unsigned char *)author1)]=end;
                        *taille += 1;
                        n1 = *taille;
                    }
                    L[index] = n1;
                    index++;
                    L[index] = -1;
                }
            }
            for (int i = 0; L[i] > -1 && i < 100; i++)
            {
                for (int k = i + 1; L[k] > -1 && k < 100; k++)
                {
                    appendEdge(L[i], L[k], node0);
                }
            }
        }
        Entree = readEntryBin(option, -1);
    }
    printf("************Fin DoListAdjDeBinHash************\n");
    return node0;
}