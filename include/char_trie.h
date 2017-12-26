#ifndef CHAR_TRIE_H
#define CHAR_TRIE_H

#include "../include/trie.h"

class Char_trie : public Trie<char32_t>{
public:
    virtual ~Char_trie() { };

    Char_trie(){};

    Char_trie(const Char_trie& orig) = default;

    /* Using the index idx, this function builds C-style string
     * corresponding to the index idx. */
    char32_t* get_cstring(size_t idx);

    /* Using index idx, this function builds a string of the type u32string
     * corresponding to the index idx. */
    std::u32string get_string(size_t idx);

    /* This function outputs the string corresponding to the index idx. */
    void print(size_t idx);

    /* The following function returns the length of the string
     * corresponding to the index idx. */
    size_t get_length(size_t idx);
};
#endif
