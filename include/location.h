#ifndef LOCATION_H
#define LOCATION_H

#include <memory>
//#include <cstddef>
/**
 * \brief The following structure describes the current position in the processed text.
 *        In the scanner's constructor, you must pass a smart pointer to the shared
 *        information about the current location.
 */
struct Location {
    char32_t* pcurrent_char; ///< pointer to the current character
    size_t    current_line;  ///< number of current line

    Location() : pcurrent_char(nullptr), current_line(1) {};
    Location(char32_t* txt) : pcurrent_char(txt), current_line(1) {};
};

using Location_ptr = std::shared_ptr<Location>;
#endif
