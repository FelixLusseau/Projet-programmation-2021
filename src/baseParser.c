#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "baseParser.h"
#include "io-utils.h"
#include "readBin.h"
#include "tps_unit_test.h"

extern int interruptFlag;

void initStructure(structureBase_t *structureBase, int authornb) {
    structureBase->year = 0;
    structureBase->titleLength = 0;
    structureBase->title[0] = '\0';
    structureBase->authornb = 0;
    for (int h = 0; h <= authornb; h++) {
        structureBase->author[h][0] = '\0';
        structureBase->authorlengths[h] = 0;
    }
}

void extractAuthor(structureBase_t *structureBase, char *line) {
    int dec = 8;
    int i = dec;
    if (line[i - 1] != '>') {
        while (line[i] != '>') {
            i++;
        }
        i++;
        dec = i;
    }
    while (line[i] != '<') {
        /* code to correct the encoding errors of the xml */
        if (line[i] == '&') {
            if (line[i + 1] == 'e' && line[i + 2] == 't') {
                structureBase->author[structureBase->authornb][i - dec] = 'd';
            } else if (line[i + 1] == 'E' && line[i + 2] == 'T') {
                structureBase->author[structureBase->authornb][i - dec] = 'D';
            } else if (line[i + 1] == 't' && line[i + 2] == 'h') {
                structureBase->author[structureBase->authornb][i - dec] = 't';
                structureBase->author[structureBase->authornb][i - dec + 1] =
                    'h';
                dec--;
            } else if (line[i + 1] == 'T' && line[i + 2] == 'H') {
                structureBase->author[structureBase->authornb][i - dec] = 'T';
                structureBase->author[structureBase->authornb][i - dec + 1] =
                    'H';
                dec--;
            } else if (line[i + 1] == 's' && line[i + 2] == 'z') {
                structureBase->author[structureBase->authornb][i - dec] = 's';
                structureBase->author[structureBase->authornb][i - dec + 1] =
                    's';
                dec--;
            } else
                structureBase->author[structureBase->authornb][i - dec] =
                    line[i + 1];
            while (line[i] != ';') {
                i++;
                dec++;
            }
            i++;
        } else {
            structureBase->author[structureBase->authornb][i - dec] = line[i];
            i++;
        }
    }
    structureBase->author[structureBase->authornb][i - dec] = '\0';
    structureBase->authorlengths[structureBase->authornb] = i - dec;
    structureBase->authornb += 1;
}

void extractYear(structureBase_t *structureBase, char *line) {
    int dec = 6;
    int i = dec;
    char yeartmp[5];
    while (line[i] != '<') {
        yeartmp[i - dec] = line[i];
        i++;
    }
    yeartmp[i - dec] = '\0';
    structureBase->year = atoi(yeartmp);
}

void extractTitle(structureBase_t *structureBase, char *line) {
    int dec = 7;
    int i = dec;
    while (line[i] != '<') {
        /* code to correct the encoding errors of the xml */
        if (line[i] == '&') {
            if (line[i + 1] == 'e' && line[i + 2] == 't') {
                structureBase->title[i - dec] = 'd';
            } else if (line[i + 1] == 'E' && line[i + 2] == 'T') {
                structureBase->title[i - dec] = 'D';
            } else if (line[i + 1] == 't' && line[i + 2] == 'h') {
                structureBase->title[i - dec] = 't';
                structureBase->title[i - dec + 1] = 'h';
                dec--;
            } else if (line[i + 1] == 'T' && line[i + 2] == 'H') {
                structureBase->title[i - dec] = 'T';
                structureBase->title[i - dec + 1] = 'H';
                dec--;
            } else if (line[i + 1] == 's' && line[i + 2] == 'z') {
                structureBase->title[i - dec] = 's';
                structureBase->title[i - dec + 1] = 's';
                dec--;
            } else
                structureBase->title[i - dec] = line[i + 1];
            while (line[i] != ';') {
                i++;
                dec++;
            }
            i++;
        } else {
            structureBase->title[i - dec] = line[i];
            i++;
        }
    }
    structureBase->titleLength = i - dec;
    structureBase->title[i - dec] = '\0';
}

error_t parseBase(options_t *options) {
    printf("Parsing...\n");
    fprintf(options->outputFile, "<binary file>\n");
    fprintf(options->outputFile, "             \n");
    int counter = 0;
    char *line = malloc(1500);
    if (line == NULL) {
        return ERROR_BASE_PARSE;
    }
    structureBase_t structureBase;
    initStructure(&structureBase, 0);
    while (fgets(line, 1500, options->inputFile) != NULL) {
        if (line[0] == '<' && line[1] == 'a' && line[2] == 'u')
            extractAuthor(&structureBase, line);
        else if (line[0] == '<' && line[1] == 't' && line[2] == 'i')
            extractTitle(&structureBase, line);
        else if (line[0] == '<' && line[1] == 'y' && line[2] == 'e')
            extractYear(&structureBase, line);
        else if (line[0] == '<' && line[1] == '/') {
            /* At the end of an entry, write into the binary */
            if (structureBase.authornb != 0) {
                counter++;
                fwrite(&structureBase,
                       2 * sizeof(int16_t) + structureBase.titleLength + 1, 1,
                       options->outputFile);
                fwrite(&structureBase.authornb, sizeof(int16_t), 1,
                       options->outputFile);
                fwrite(&structureBase.authorlengths,
                       structureBase.authornb * sizeof(int8_t), 1,
                       options->outputFile);
                for (int m = 0; m < structureBase.authornb; m++) {
                    fwrite(structureBase.author[m],
                           structureBase.authorlengths[m] + 1, 1,
                           options->outputFile);
                }
            }
            if (interruptFlag == 1) {
                break;
            }
            initStructure(&structureBase, structureBase.authornb);
        }
    }
    fseek(options->outputFile, 14, SEEK_SET);
    fwrite(&counter, sizeof(int), 1, options->outputFile);
    free(line);
    fclose(options->outputFile);
    options->outputFile = NULL;
    return OK;
}