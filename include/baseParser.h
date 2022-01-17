#ifndef BASE_PARSER_H
#define BASE_PARSER_H

#include "program.h"

/**
 * @brief initialise the structure with the NULL values corresponding to the
 * types
 *
 * @param structureBase
 * @param authornb
 */
void initStructure(structureBase_t *structureBase, int authornb);

/**
 * @brief extract the author from a line containing the <author> tag
 *
 * @param structureBase
 * @param line
 */
void extractAuthor(structureBase_t *structureBase, char *line);

/**
 * @brief extract the year from a line containing the <year> tag
 *
 * @param structureBase
 * @param line
 */
void extractYear(structureBase_t *structureBase, char *line);

/**
 * @brief extract the title from a line containing the <title> tag
 *
 * @param structureBase
 * @param line
 */
void extractTitle(structureBase_t *structureBase, char *line);

/**
 * @brief function to extract data from the xml base to the structureBase_t
 * structure then written in the binary file
 *
 * @param options
 * @return error_t
 */
error_t parseBase(options_t *options);

#endif