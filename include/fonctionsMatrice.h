#ifndef FONCTIONSMATRICE_H
#define FONCTIONSMATRICE_H

//Structure pour faire une liste adjacente.
typedef struct node
{
    char nodeName[50];
    int nodeNumber;
    int marqueurNode;
    struct edges *nodesEdges;
    struct node *nextNode;
    int edges[100]={0};
} node;

node *DoAdjacentList(FILE *binaire)
{
    structureBase_t *infoArticle;
    node *node1 = malloc(sizeof(node));
    node1->nodeNumber = 1;
    node *currentNode = node1;
    int currentNumber = 1;
    while (fread(infoArticle, sizeof(structureBase_t), 1, binaire) != 0)
    {
        for (int i = 0; i < 10; i++)
        {
            if (author[i] == NULL)
            {
                break;
            }
            int n = AuthorInList(author[i], node1);
            if (n == 0)
            {
                currentNumber += 1;
                node *newNode = malloc(sizeof(node));
                newNode->nodeName = author[i];
                newNode->nodeNumber = currentNumber;
                currentNode->nextNode = newNode;
                currentNode = newNode;
            }
            for (int k = 0; k < i; k++)
            {
                int n2 = AuthorInList(author[k], node1);
                appendEdges(n,n2);
            }
        }
    }
    return node1;
}

/**test la présence de l'auteur dans la liste d'adjacence
   renvoi le numéros de son sommet si il est présent
   renvoi 0 sinon
*/
int AuthorInList(char *author, node *node1)
{
    node *currentNode = node1;
    while (currentNode->nextNode != NULL)
    {
        if (strcmp(currentNode->nodeName, auteur) == 0)
        {
            return currentNode->nodeNumber;
        }
        currentNode = sommecurrentNodetActuel->nextNode;
    }
    return 0;
}

node *GoToNode(int n, node *node1)
{
    node *currentNode = node1;
    for (int k = 0; k < n; k++)
    {
        currentNode = currentNode->nextNode;
    }
    return currentNode;
}

void appendEdges(int n1,int n2,node *node1)
{
    node *Node1=GoToNode(n1,node1);
    node *Node2=GoToNode(n2,node1);
    int index=0;
    while(currentNode->Edges[index]!=0){
        index++;
    }
    currentNode->Edges[index]=


}

#endif