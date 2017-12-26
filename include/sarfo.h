#ifndef SARFO_H
#define SARFO_H

#include "../include/abstract_scaner.h"
#include "../include/error_count.h"
#include "../include/location.h"
#include <string>"

enum Code : unsigned short {
    None,  Unknown,  MUL,   
    PLUS,  MINUS,    DIV,   
    POW,   SEMICOLON,ASSIGN,
    NUMBER
};

struct Lexem_info{
    Code code;
    union{
        size_t    ident_index;
unsigned __int128 int_val;
    };
};

class SARFO : public Abstract_scaner<Lexem_info> {
public:
    SARFO() = default;
    SARFO(Location* location, const Errors_and_tries& et) :
        Abstract_scaner<Lexem_info>(location, et) {};
    SARFO(const SARFO& orig) = default;
    virtual ~SARFO() = default;
    virtual Lexem_info current_lexem();
private:
    enum Automaton_name{
        A_start,     A_unknown, A_id, 
        A_delimiter, A_number
    };
    Automaton_name automaton; /* current automaton */

   typedef bool (SARFO::*Automaton_proc)();
    /* This is the type of pointer to the member function that implements the
     * automaton that processes the lexeme. This function must return true if
     * the lexeme is not yet parsed, and false otherwise. */

    typedef void (SARFO::*Final_proc)();
    /* And this is the type of the pointer to the member function that performs
     * the necessary actions in the event of an unexpected end of the lexeme. */

    static Automaton_proc procs[];
    static Final_proc     finals[];

    /* Lexeme processing functions: */
    bool start_proc();  bool unknown_proc();   
    bool id_proc();     bool delimiter_proc(); 
    bool number_proc();
    /* functions for performing actions in case of an
     * unexpected end of the token */
    void none_proc();         void unknown_final_proc();   
    void id_final_proc();     void delimiter_final_proc(); 
    void number_final_proc();
};
#endif
