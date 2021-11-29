#ifndef IO_UTILS_H
#define IO_UTILS_H

#include "program.h"

/**
 * @brief Ouvrir les fichiers donnés en arguments
 * 
 * @param options 
 * @return int 
 */

int openFiles(options_t * options, char * openMode);

/**
 * @brief Fermer les fichiers donnés en arguments
 * 
 * @param options 
 * @return int 
 */

int closeFiles(options_t * options);

#endif