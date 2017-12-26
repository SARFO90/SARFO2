#ifndef GET_INIT_STATE_H
#define GET_INIT_STATE_H

struct State_for_char{
    unsigned st;
    char32_t c;
};

/* The function get_init_state initializes a finite automaton. Namely, this function
 * looking for the character sym in the table sts. Here sts is an array of
 * pairs (state, character), and number of elements in sts is n. The search is
 * performed in accordance with the binary search algorithm B from the section 6.2.1
 * of "Knuth D.E. The Art of Computer programming. V.3. Sorting and search. 2nd ed.
 * --- Addison Wesley, 1998.". */
int get_init_state(char32_t sym, const State_for_char sts[], int n);
#endif
