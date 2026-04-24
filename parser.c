/**********************************************************************/
/* lab 1 DVG C01 - Parser OBJECT                                      */
/**********************************************************************/

/**********************************************************************/
/* Include files                                                      */
/**********************************************************************/
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

/**********************************************************************/
/* Other OBJECT's METHODS (IMPORTED)                                  */
/**********************************************************************/
 #include "keytoktab.h"          /* when the keytoktab is added   */
 #include "lexer.h"              /* when the lexer     is added   */
 #include "symtab.h"             /* when the symtab    is added   */
 #include "optab.h"              /* when the optab     is added   */

/**********************************************************************/
/* OBJECT ATTRIBUTES FOR THIS OBJECT (C MODULE)                       */
/**********************************************************************/
#define DEBUG 0 
static int  lookahead=0;
static int  is_parse_ok=1;

/**********************************************************************/
/* RAPID PROTOTYPING - simulate the token stream & lexer (get_token)  */
/**********************************************************************/
/* define tokens + keywords NB: remove this when keytoktab.h is added */
/**********************************************************************/
/**********************************************************************/
/* Simulate the token stream for a given program                      */
/**********************************************************************/

/**********************************************************************/
/*  Simulate the lexer -- get the next token from the buffer          */
/**********************************************************************/


/**********************************************************************/
/*  PRIVATE METHODS for this OBJECT  (using "static" in C)            */
/**********************************************************************/
static void in(char* s)
{
    if(DEBUG) printf("\n *** In  %s", s);
}
static void out(char* s)
{
    if(DEBUG) printf("\n *** Out %s", s);
}
/**********************************************************************/
/* The Parser functions                                               */
/**********************************************************************/
static void match(int t)
{
    if(DEBUG) printf("\n --------In match expected: %s, found: %s",
                    tok2lex(t), tok2lex(lookahead));
    if (lookahead == t) lookahead = get_token();
    else {
    is_parse_ok=0;
    printf("\nSYNTAX: Symbol expected %s found: %s\n",
              tok2lex(t), get_lexeme());
    }
    
}

/**********************************************************************/
/* The grammar functions                                              */
/**********************************************************************/
static void stat_part();
static void program_header();
static void var_part();
static void var_dec_list();
static void var_dec();
static void id_list();
static void type();
static void stat_list();
static void stat();
static void assign_stat();
static toktyp expr();
static toktyp term();
static toktyp factor();
static toktyp operand();

static void prog()
{
    program_header(); var_part(); stat_part(); match('.');
}
static void program_header()
{
    in("program_header");
    match(program);  
    addp_name(get_lexeme()); 
    match(id); 
    match('('); 
    match(input);
    match(','); 
    match(output); 
    match(')'); 
    match(';');
    out("program_header");
}

static void var_part()
{
    in("var_part");
    match(var); 
    var_dec_list();
    out("var_part");
}

static void var_dec_list()
{
    in("var_dec_list");
    var_dec();
    if(lookahead == id)
        var_dec_list();
    
    out("var_dec_list");

}

static void var_dec()
{
    in("var_dec");
    id_list(); match(':'); type(); match(';');
    out("var_dec");
}

static void id_list()
{
    in("id_list");
    
    if (lookahead == id) 
    {
        char* curr_name = get_lexeme();
        
        if(find_name(curr_name))
        {
            is_parse_ok = 0;
            printf("\nSEMANTIC: ID already declared: %s", curr_name);
        }
        else
        {
            addv_name(curr_name);
        }
        
        match(id); 
    }
    else 
    {
        match(id);
    }
    
    if(lookahead == ',')
    {
        match(',');
        id_list();
    }
    
    out("id_list");
}
static void type()
{
    in("type");

    if (lookahead == integer || lookahead == real || lookahead == boolean) {
        toktyp curr_type = lookahead;
        setv_type(curr_type);
        match(curr_type);
    } 
    else {
        printf("\nSYNTAX: Type name expected found %4s\n", get_lexeme());
        is_parse_ok = 0;
        
        setv_type(error);
        
    }

    out("type");
}
static void stat_part(){
    in("stat_part");
    match(begin);
    stat_list();
    match(end);
    out("stat_part");

}

static void stat_list(){
    in("stat_list");
    stat();

    while(lookahead == ';'){
        match(';');
        stat();
    }
    out("stat_list");
}


static void stat(){
    in("stat");
    assign_stat();
    out("stat");
}


static void assign_stat(){
    in("assign_stat");
    char* var_name = get_lexeme();
    toktyp lhs_type;

    if (lookahead == id) {
        lhs_type = get_ntype(var_name);
        
        if (lhs_type == undef) {
            printf("\nSEMANTIC: ID NOT declared: %s\n", var_name);
            is_parse_ok = 0;
        }
    } 
    else {
        lhs_type = error;
    }

    match(id);
    match(assign);
    
    toktyp rhs_type = expr();

    if(lhs_type != rhs_type)
    {
        
       
        printf("\nSEMANTIC: Assign types: %s := %s\n", tok2lex(lhs_type), tok2lex(rhs_type));
        is_parse_ok = 0;
        
    }
    
    out("assign_stat");
}

static toktyp expr(){
    in("expr");
    toktyp left_type = term(); 
    while(lookahead == '+'){
        int op = lookahead;
        match(lookahead);
        toktyp right_type = term();
        left_type = get_otype(op, left_type, right_type);
    }

    out("expr");
    return left_type;
}

static toktyp term(){
    in("term");
    toktyp left_type = factor();
    while(lookahead == '*'){
        int op = lookahead;
        match(lookahead);
        toktyp right_type = factor();
        left_type = get_otype(op, left_type, right_type);
    }

    out("term");
    return  left_type;
}

static toktyp factor()
{
    toktyp type = undef;
    in("factor");
    if(lookahead == '('){
            match('(');
            type = expr();
            match(')');
    }
    else {
            type = operand();
    }
    
    out("factor");
    return type;
}

static toktyp operand(){
    toktyp type = undef;
    in("operand");
    
    if(lookahead == id){
        char* var_name = get_lexeme();
        type = get_ntype(var_name);
        
        
        if (type == undef) {
            printf("\nSEMANTIC: ID NOT declared: %s \n", var_name);
            is_parse_ok = 0;
        }
        
        match(id);
    }
    else if(lookahead == number) {
        type = integer; 
        match(number);
    }
    else {
        printf("\nSYNTAX: Operand expected\n");
        is_parse_ok = 0;

    }
    
   out("operand");
   return type; 
}


/**********************************************************************/
/*  PUBLIC METHODS for this OBJECT  (EXPORTED)                        */
/**********************************************************************/

int parser()
{
    
    in("parser");
    lookahead = get_token();       
    if(lookahead == '$')
    {
        printf("WARNING: Input file is empty\n");
        is_parse_ok = 0;
    }
    else
    {
        prog();               
        out("parser");
        if (lookahead != '$') {
                printf("\nSYNTAX: Extra symbols after end of parse!\n");
                while (lookahead != '$') {
                    printf("%s ", get_lexeme());
                    lookahead = get_token();
                }
                printf("\n");
            }
    }
    p_symtab();
    return is_parse_ok;             
}

/**********************************************************************/
/* End of code                                                        */
/**********************************************************************/
