#ifndef ABSTRACT_SCANER_H
#define ABSTRACT_SCANER_H

#include <string>
#include <memory>
#include "../include/error_count.h"
#include "../include/trie.h"
#include "../include/location.h"
#include "../include/errors_and_tries.h"

template<typename Lexem_type>
class Scaner{
public:
    Scaner<Lexem_type>() = default;
    Scaner(Location_ptr location, const Errors_and_tries& et);
    Scaner(const Scaner<Lexem_type>& orig) = default;
    virtual ~Scaner() = default;
    /*  Function back() return the current lexem into the input stream. */
    void back();
    /* Function current_lexem() returns information about current lexem,
     * i.e. returns a lexem code and a lexem value. */
    virtual Lexem_type current_lexem() = 0;
    /* Function lexem_begin_line_number() returns the line number
     * at which the lexem starts. */
    size_t lexem_begin_line_number();
protected:
    int                          state; /* the current state of the current automaton */

    Location_ptr                 loc;
    char32_t*                    lexem_begin; /* pointer to the lexem begin */
    char32_t                     ch;          /* current character */

    /* set of categories for the current character */
    uint64_t                     char_categories;

    /* intermediate value of the lexem information */
    Lexem_type                   token;

    /* the line number at which the current lexem starts */
    size_t                       lexem_begin_line;

    /* a pointer to a class that counts the number of errors: */
    std::shared_ptr<Error_count> en;
    /* a pointer to the prefix tree for identifiers: */
    std::shared_ptr<Char_trie>   ids;
    /* a pointer to the prefix tree for string literals: */
    std::shared_ptr<Char_trie>   strs;

    /* buffer for writing the processed identifier or string: */
    std::u32string               buffer;
};

template<typename Lexem_type>
Scaner<Lexem_type>::Scaner(Location_ptr location, const Errors_and_tries& et){
    ids = et.ids_trie; strs = et.strs_trie; en = et.ec;
    loc = location;
    lexem_begin = location->pcurrent_char;
    lexem_begin_line = 1;
}

template<typename Lexem_type>
void Scaner<Lexem_type>::back(){
    loc->pcurrent_char = lexem_begin;
    loc->current_line =  lexem_begin_line;
}

template<typename Lexem_type>
size_t Scaner<Lexem_type>::lexem_begin_line_number(){
    return lexem_begin_line;
}
#endif
