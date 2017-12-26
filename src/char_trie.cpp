#include "../include/char_conv.h"
#include "../include/char_trie.h"
#include <vector>
#include <map>
#include <utility>
#include <algorithm>
#include <string>
#include <set>

char32_t* Char_trie::get_cstring(size_t idx){
    size_t id_len = node_buffer[idx].path_len;
    char32_t* p = new char32_t[id_len + 1];
    p[id_len] = 0;
    size_t current = idx;
    size_t i       = id_len-1;
    /* Since idx is the index of the element in node_buffer containing the last
     * character of the inserted string, and each element of the vector node_buffer
     * contains the field parent that points to the element with the previous
     * character of the string, then to get the inserted string, which corresponds
     * to the index idx, as an array of characters, it is necessary to walk from
     * the element with index idx to the root. The characters of the inserted
     * string will be read from the end to the beginning. */
    for( ; current; current = node_buffer[current].parent){
        p[i--] = node_buffer[current].c;
    }
    return p;
}

std::u32string Char_trie::get_string(size_t idx){
    char32_t* p = get_cstring(idx);
    std::u32string s = std::u32string(p);
    delete [] p;
    return s;
}

void Char_trie::print(size_t idx){
    std::u32string s32 = get_string(idx);
    std::string    s8  = u32string_to_utf8(s32);
    printf("%s",s8.c_str());
}

size_t Char_trie::get_length(size_t idx){
    return node_buffer[idx].path_len;
}
