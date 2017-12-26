#ifndef CHAR_CONV_H
#define CHAR_CONV_H

#include <string>

/**
\param [in] utf8str --- UTF-8 string with terminating null character

\return value of the type std::u32string, representing the same string,
but in the encoding UTF-32
*/
std::u32string utf8_to_u32string(const char* utf8str);

/**
\param [in] u32str --- string in the encoding UTF-32

\return value of the type std::string, representing the same string,
but in the encoding UTF-8
*/
std::string u32string_to_utf8(const std::u32string& u32str);

/**
\param [in] c --- character in the encoding UTF-32

\return value of the type std::string, consisting of bytes, representing
the same character, but in the encoding UTF-8.
*/
std::string char32_to_utf8(const char32_t c);
#endif
