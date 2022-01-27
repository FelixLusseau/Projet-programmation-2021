#include "searchingFunctions.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/signal.h>

#include "analyseGraph.h"
#include "baseParser.h"
#include "makeGraph.h"
#include "program.h"
#include "readBin.h"

extern int interruptFlag;

int showAuthors(options_t *options, node *node0, int author0or1) {
    fseek(options->outputFile, 28, SEEK_SET);
    int exact = 0;
    if (node0 == NULL) {
        return ERROR_NODE_EQ_NULL;
    }
    node *currentNode = node0;
    char *authortmp = currentNode->author;
    int counter = 0;
    printf("Authors containing \"%s\" in their name : \n",
           options->authorNames[author0or1]);
    while (currentNode != NULL) {
        if (strstr(currentNode->author, options->authorNames[author0or1])) {
            if (strcmp(currentNode->author, options->authorNames[author0or1]) ==
                0) {
                exact = 1;
                authortmp = currentNode->author;
                printf(" - %s\n", currentNode->author);
                counter = 1;
                break;
            }
            authortmp = currentNode->author;
            printf(" - %s\n", currentNode->author);
            counter++;
        }
        currentNode = currentNode->nextNode;
        if (interruptFlag == 1)
            break;
    }
    if (counter == 0) {
        printf("\n\33[0;33mNo author containing \"\33[0;31m%s\33[0m\33[0;33m\" "
               "in his name ! \33[0m",
               options->authorNames[author0or1]);
    }
    if (counter == 1 || exact == 1) {
        options->authorNames[author0or1] = authortmp;
    }
    printf("\n\n");
    return counter;
}

error_t chooseAuthor(options_t *options, node *node0, int author0or1) {
    int count;
begin:
    count = showAuthors(options, node0, author0or1);
    int c;
    fseek(options->outputFile, 28, SEEK_SET);
    char *exitChar = "Ok";
    if (count == 0) {
        return ERROR_NO_AUTHOR;
    } else if (count != 1) {
        printf("%i authors found ! Which one do you want to see ?\n"
               "Enter his complete name : ",
               count);
        exitChar = fgets(options->authorNames[author0or1], 100, stdin);
        printf("\n");
        if (exitChar == NULL) {
            return ERROR_SHOW_ARTICLES;
        }
        options->authorNames[author0or1]
                            [strlen(options->authorNames[author0or1]) - 1] =
            '\0';
        c = showAuthors(options, node0, author0or1);
        if (c > 1) {
            fprintf(stderr,
                    "This is not enough precise ! There are %i authors "
                    "containing \"\33[0;31m%s\33[0m\" in their name !\n",
                    c, options->authorNames[author0or1]);
            goto begin;
        } else if (c == 0) {
            fprintf(stderr, "\33[0;31mInvalid answer !\33[0m\n");
            return ERROR_CHOOSE_AUTHOR;
        }
    }
    return OK;
}

error_t showArticles(options_t *options, node *node0, int year) {
    int exitCode = chooseAuthor(options, node0, 0);
    if (exitCode != OK)
        return exitCode;
    else if (exitCode == ERROR_NO_AUTHOR) {
        return OK;
    }
    int count = 0;
    int16_t precAuthornb = 0;
    structureBase_t structureBase;
    printf("Articles of %s", options->authorNames[0]);
    if (year > 0) {
        printf(" in %i", year);
    }
    printf(" : \n");
    while (1) {
        initStructure(&structureBase, precAuthornb);
        readStructure(options, &structureBase, precAuthornb);
        precAuthornb = structureBase.authornb;
        if (structureBase.authornb == 0 || interruptFlag == 1)
            break;
        for (int k = 0; k < structureBase.authornb; k++) {
            if (strstr(structureBase.author[k], options->authorNames[0])) {
                if ((year > 0 && structureBase.year == year) || year == 0)
                    printf(" - %s\n", structureBase.title);
                count++;
                break;
            }
        }
        precAuthornb = structureBase.authornb;
    }
    if (count == 0 && year == 0) {
        fprintf(stderr, "\nInvalid author name given ! \n");
        return ERROR_SHOW_ARTICLES;
    }
    if (count == 0 && year != 0) {
        fprintf(stderr, "\nInvalid author name or year given ! \n");
        return ERROR_SHOW_ARTICLES;
    }
    if (interruptFlag != 1)
        printf("\n\n\33[0;32mComplete ! \33[0m\n");
    return OK;
}