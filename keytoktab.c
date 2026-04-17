/**********************************************************************/
/* lab 1 DVG C01 - Driver OBJECT                                      */
/**********************************************************************/

/**********************************************************************/
/* Include files                                                      */
/**********************************************************************/
#include <stdbool.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

/**********************************************************************/
/* Other OBJECT's METHODS (IMPORTED)                                  */
/**********************************************************************/
#include "keytoktab.h"

/**********************************************************************/
/* OBJECT ATTRIBUTES FOR THIS OBJECT (C MODULE)                       */
/**********************************************************************/
/**********************************************************************/
/* type definitions                                                   */
/**********************************************************************/
typedef struct tab {
	char 	* text;
	int 	token;
}tab;

/**********************************************************************/
/* data objects (tables)                                              */
/**********************************************************************/
static tab tokentab[ ] = {
    {"id", 	            id},
    {"number",      number},
    {":=", 	        assign},
    {"undef", 	     undef},
    {"predef",      predef},
    {"tempty",      tempty},
    {"error",        error},
    {"type",           typ},
    {"$",              '$'},
    {"(",              '('},
    {")",              ')'},
    {"*",              '*'},
    {"+",              '+'},
    {",",              ','},
    {"-",              '-'},
    {".",              '.'},
    {"/",              '/'},
    {":",              ':'},
    {";",              ';'},
    {"=",              '='},
    {"TERROR", 	    nfound}
};


static tab keywordtab[ ] = {
	{"program", 	program},
	{"input", 	      input},
	{"output", 	     output},
	{"var", 	        var},
	{"begin", 	      begin},
	{"end", 	        end},
	{"boolean", 	boolean},
	{"integer", 	integer},
	{"real", 	       real},
	{"KERROR", 	     nfound}
};

static void print_token(tab token)
{
    printf("  %s   %d\n",token.text,token.token);
}

/**********************************************************************/
/*  PUBLIC METHODS for this OBJECT  (EXPORTED)                        */
/**********************************************************************/
/**********************************************************************/
/* Display the tables                                                 */
/**********************************************************************/
void p_toktab()
{
    printf("***   Token table   ***\n");
    tab* curr_tok = &tokentab[0];
    while(curr_tok->token != nfound)
    {
        print_token(*curr_tok);
        curr_tok++;
    } 

    printf("\n\n");

    curr_tok = &keywordtab[0];
    printf("***   Keyword Table   ***\n");
    while(curr_tok->token != nfound)
    {
        print_token(*curr_tok);
        curr_tok++;
    } 
    printf("\n");

}

/**********************************************************************/
/* lex2tok - convert a lexeme to a token                              */
/**********************************************************************/
toktyp lex2tok(char * fplex)
{
    int i = 0;
    while(true)
    {
        tab curr_tok =tokentab[i];
        if(strcmp(curr_tok.text, fplex))
            return curr_tok.token;

        else if(curr_tok.token == nfound)
                break;
        i++;
    }

    i=0;

    while(true)
    {
        tab curr_key =keywordtab[i];
        if(strcmp(curr_key.text, fplex))
            return curr_key.token;

        else if(curr_key.token == nfound)
                break;
        i++;
    }
    return nfound;

}

/**********************************************************************/
/* key2tok - convert a keyword to a token                             */
/**********************************************************************/
toktyp key2tok(char * fplex)
{
    int i=0;
    while(keywordtab[i].token !=nfound){

        if(strcmp(keywordtab[i].text, fplex) == 0)
            return keywordtab[i].token;

        i++;

    }

    return nfound;

    printf("\n *** TO BE DONE");  return 0;
}

/**********************************************************************/
/* tok2lex - convert a token to a lexeme                              */
/**********************************************************************/
char * tok2lex(toktyp ftok)
{

    int i=0;
    while (tokentab[i].token != nfound) {
        if (tokentab[i].token == ftok) {
            return tokentab[i].text;
        }
        i++;
    }

    i=0;
    while (keywordtab[i].token != nfound) {
        if (keywordtab[i].token == ftok) {
            return keywordtab[i].text;
        }
        i++;
    }

    return "TERROR";
    printf("\n *** TO BE DONE");  return 0;
}


/**********************************************************************/
/* End of code                                                        */
/**********************************************************************/
