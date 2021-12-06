#ifndef IO_UTILS_H
#define IO_UTILS_H

#include "program.h"

/**
 * @brief implement action when CTRL+C is pressed
 * 
 */
void handleSignal();

/**
 * @brief implement CTRL+C personnalised interruption
 * 
 */
void initSigaction();

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

/**
 * @brief convert error from error_t to string
 * 
 * @param err 
 * @return const char* 
 */
const char * errorToString(error_t err);

#endif