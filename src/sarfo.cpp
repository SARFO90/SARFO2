#include "../include/sarfo.h"
#include "../include/get_init_state.h"
#include "../include/search_char.h"
#include "../include/belongs.h"
#include <set>
#include <string>
#include <vector>
#include "../include/operations_with_sets.h"
SARFO::Automaton_proc SARFO::procs[] = {
    &SARFO::start_proc(),  &SARFO::unknown_proc(),   
    &SARFO::id_proc(),     &SARFO::delimiter_proc(), 
    &SARFO::number_proc()
};

SARFO::Final_proc SARFO::finals[] = {
    &SARFO::none_proc(),         &SARFO::unknown_final_proc(),   
    &SARFO::id_final_proc(),     &SARFO::delimiter_final_proc(), 
    &SARFO::number_final_proc()
};

enum Category {
    SPACES,       DELIMITER_BEGIN, 
    NUMBER_BEGIN, IDENTIFIER0,     
    IDENTIFIER1,  Other
};

static const std::map<char32_t, uint8_t> categories_table = {
    {'\0', 1},   {'\X01', 1}, {'\X02', 1}, {'\X03', 1}, 
    {'\X04', 1}, {'\X05', 1}, {'\X06', 1}, {'\a', 1},   
    {'\b', 1},   {'\t', 1},   {'\n', 1},   {'\v', 1},   
    {'\f', 1},   {'\r', 1},   {'\X0e', 1}, {'\X0f', 1}, 
    {'\X10', 1}, {'\X11', 1}, {'\X12', 1}, {'\X13', 1}, 
    {'\X14', 1}, {'\X15', 1}, {'\X16', 1}, {'\X17', 1}, 
    {'\X18', 1}, {'\X19', 1}, {'\X1a', 1}, {'\X1b', 1}, 
    {'\X1c', 1}, {'\X1d', 1}, {'\X1e', 1}, {'\X1f', 1}, 
    {' ', 1},    {'*', 2},    {'+', 2},    {'-', 2},    
    {'/', 2},    {'0', 20},   {'1', 20},   {'2', 20},   
    {'3', 20},   {'4', 20},   {'5', 20},   {'6', 20},   
    {'7', 20},   {'8', 20},   {'9', 20},   {':', 2},    
    {';', 2},    {'A', 24},   {'B', 24},   {'C', 24},   
    {'D', 24},   {'E', 24},   {'F', 24},   {'G', 24},   
    {'H', 24},   {'I', 24},   {'J', 24},   {'K', 24},   
    {'L', 24},   {'M', 24},   {'N', 24},   {'O', 24},   
    {'P', 24},   {'Q', 24},   {'R', 24},   {'S', 24},   
    {'T', 24},   {'U', 24},   {'V', 24},   {'W', 24},   
    {'X', 24},   {'Y', 24},   {'Z', 24},   {'_', 8},    
    {'a', 24},   {'b', 24},   {'c', 24},   {'d', 24},   
    {'e', 24},   {'f', 24},   {'g', 24},   {'h', 24},   
    {'i', 24},   {'j', 24},   {'k', 24},   {'l', 24},   
    {'m', 24},   {'n', 24},   {'o', 24},   {'p', 24},   
    {'q', 24},   {'r', 24},   {'s', 24},   {'t', 24},   
    {'u', 24},   {'v', 24},   {'w', 24},   {'x', 24},   
    {'y', 24},   {'z', 24}
};


uint64_t get_categories_set(char32_t c){
    auto it = categories_table.find(c);
    if(it != categories_table.end()){
        return it->second;
    }else{
        return 1ULL << Other;
    }
}
bool SARFO::start_proc(){
    bool t = true;
    state = -1;
    /* For an automaton that processes a token, the state with the number (-1) is
     * the state in which this automaton is initialized. */
    if(belongs(SPACES, char_categories)){
        loc->current_line += U'\n' == ch;
        return t;
    }
    lexem_begin_line = loc->current_line;
    if(belongs(DELIMITER_BEGIN, char_categories)){
        (loc->pcurrent_char)--; automaton = A_delimiter;
        state = -1;
        return t;
    }

    if(belongs(NUMBER_BEGIN, char_categories)){
        (loc->pcurrent_char)--; automaton = A_number;
        state = 0;
        val_=0; tokencode=NUMBER;
        return t;
    }

    if(belongs(IDENTIFIER0, char_categories)){
        (loc->pcurrent_char)--; automaton = A_id;
        state = 0;
        return t;
    }

    automaton = A_unknown;
    return t;
}

bool SARFO::unknown_proc(){
    return belongs(Other, char_categories);
}

static const std::set<size_t> final_states_for_identiers = {
    1
};

bool SARFO::id_proc(){
    bool t             = true;
    bool there_is_jump = false;
    switch(state){
        case 0:
            if(belongs(IDENTIFIER0, char_categories)){
                state = 1;
                there_is_jump = true;
            }

            break;
        case 1:
            if(belongs(IDENTIFIER1, char_categories)){
                state = 1;
                there_is_jump = true;
            }

            break;
        default:
            ;
    }

    if(!there_is_jump){
        t = false;
        if(!is_elem(state, final_states_for_identiers)){
            printf("At line %zu unexpectedly ended identifier.", loc->current_line);
            en->increment_number_of_errors();
        }
        token.ident_index = ids -> insert(buffer);
    }

    return t;
}

static const State_for_char init_table_for_delimiters[] ={
    {0, U'*'}, {2, U'+'}, {3, U'-'}, {4, U'/'}, {6, U':'}, 
    {5, U';'}
};

struct Elem {
    /** A pointer to a string of characters that can be crossed. */
    char32_t*       symbols;
    /** A lexeme code. */
    Code code;
    /** If the current character matches symbols[0], then the transition to the state
     *  first_state;
     *  if the current character matches symbols[1], then the transition to the state
     *  first_state + 1;
     *  if the current character matches symbols[2], then the transition to the state
     *  first_state + 2, and so on. */
    uint16_t        first_state;
};

static const Elem delim_jump_table[] = {
    {const_cast<char32_t*>(U"*"), MUL, 1},      
    {const_cast<char32_t*>(U""), POW, 0},       
    {const_cast<char32_t*>(U""), PLUS, 0},      
    {const_cast<char32_t*>(U""), MINUS, 0},     
    {const_cast<char32_t*>(U""), DIV, 0},       
    {const_cast<char32_t*>(U""), SEMICOLON, 0}, 
    {const_cast<char32_t*>(U"="), Unknown, 7},  
    {const_cast<char32_t*>(U""), ASSIGN, 0}
};

bool SARFO::delimiter_proc(){
    bool t = false;
    if(-1 == state){
        state = get_init_state(ch, init_table_for_delimiters,
                               sizeof(init_table_for_delimiters)/sizeof(State_for_char));
        token.code = delim_jump_table[state].code;
        t = true;
        return t;
    }
    Elem elem = delim_jump_table[state];
    token.code = delim_jump_table[state].code;
    int y = search_char(ch, elem.symbols);
    if(y != THERE_IS_NO_CHAR){
        state = elem.first_state + y; t = true;
    }
    return t;
}

static const std::set<size_t> final_states_for_numbers = {
    1
};

bool SARFO::number_proc(){
    bool t             = true;
    bool there_is_jump = false;
    switch(state){
        case 0:
            if(belongs(NUMBER_BEGIN, char_categories)){
                val_=val_*10+digit_to_int(ch);
                state = 1;
                there_is_jump = true;
            }

            break;
        case 1:
            if(belongs(NUMBER_BEGIN, char_categories)){
                val_=val_*10+digit_to_int(ch);
                state = 1;
                there_is_jump = true;
            }

            break;
        default:
            ;
    }

    if(!there_is_jump){
        t = false;
        if(!is_elem(state, final_states_for_numbers)){
            printf("At line %zu unexpectedly ended the number.", loc->current_line);
            en->increment_number_of_errors();
        }
        int_val=val_;
    }

    return t;
}

void SARFO::none_proc(){
    /* This subroutine will be called if, after reading the input text, it turned
     * out to be in the A_start automaton. Then you do not need to do anything. */
}

void SARFO::unknown_final_proc(){
    /* This subroutine will be called if, after reading the input text, it turned
     * out to be in the A_unknown automaton. Then you do not need to do anything. */
}

void SARFO::id_final_proc(){
    if(!is_elem(state, final_states_for_identiers)){
        printf("At line %zu unexpectedly ended identifier.", loc->current_line);
        en->increment_number_of_errors();
    }
    token.ident_index = ids -> insert(buffer);
}

void SARFO::delimiter_final_proc(){
        
    token.code = delim_jump_table[state].code;
    
}

void SARFO::number_final_proc(){
    if(!is_elem(state, final_states_for_numbers)){
        printf("At line %zu unexpectedly ended the number.", loc->current_line);
        en->increment_number_of_errors();
    }
    int_val=val_;
}

Lexem_info SARFO::current_lexem(){
    automaton = A_start; token.code = None;
    lexem_begin = loc->pcurrent_char;
    bool t = true;
    while((ch = *(loc->pcurrent_char)++)){
        char_categories = get_categories_set(ch); //categories_table[ch];
        t = (this->*procs[automaton])();
        if(!t){
            /* We get here only if the lexeme has already been read. At the same time,
             * the current automaton reads the character immediately after the end of
             * the token read, based on this symbol, it is decided that the token has
             * been read and the transition to the next character has been made.
             * Therefore, in order to not miss the first character of the next lexeme,
             * we need to decrease the pcurrent_char pointer by one. */
            (loc->pcurrent_char)--;
            return token;
        }
    }
    /* Here we can be, only if we have already read all the processed text. In this
     * case, the pointer to the current symbol indicates a byte, which is immediately
     * after the zero character, which is a sign of the end of the text. To avoid
     * entering subsequent calls outside the text, we need to go back to the null
     * character. */
    (loc->pcurrent_char)--;
    /* Further, since we are here, the end of the current token (perhaps unexpected)
     * has not yet been processed. It is necessary to perform this processing, and,
     * probably, to display any diagnostics. */
    (this->*finals[automaton])();
    return token;
}


