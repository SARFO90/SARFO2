#ifndef SEARCH_CHAR_H
#define SEARCH_CHAR_H

#define THERE_IS_NO_CHAR (-1)
/**
 * \brief This function searches for a given character of type char32_t
 *        in a string consisting of characters of this type and ending
 *        with a null character.
 *
 * \param [in] c     The wanted character.
 * \param [in] array The string in which the symbol is searched for.
 * \return           Offset (in characters) from the beginning of the line, if the
 *                   desired character is in the string, and (-1) otherwise
 */
int search_char(char32_t c, const char32_t* array);
#endif
