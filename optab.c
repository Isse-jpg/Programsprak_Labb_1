/**********************************************************************/
/* lab 1 DVG C01 - Operator Table OBJECT                              */
/**********************************************************************/

/**********************************************************************/
/* Include files                                                      */
/**********************************************************************/
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
#define NENTS 4

static int optab[][NENTS] = {
   {'+', integer, integer, integer},
   {'+', real,    real,    real},
   {'+', integer, real,    real},
   {'+', real,    integer, real},
   {'*', integer, integer, integer},
   {'*', real,    real,    real},
   {'*', integer, real,    real},
   {'*', real,    integer, real},
   {'-', integer, integer, integer},
   {'-', integer, real,    real},
   {'-', real,    integer, real},
   {'-', real,    real,    real},
    // kan vara både real eller int
   {'/', integer, integer, undef},
   {'/', integer, real,    real},
   {'/', real,    integer, real},
   {'/', real,    real,    real},
   {'$', undef,   undef,   undef},
   };

/**********************************************************************/
/*  PRIVATE METHODS for this OBJECT  (using "static" in C)            */
/**********************************************************************/
/**********************************************************************/
/* PRIVATE METHODS for this OBJECT  (using "static" in C)            */
/**********************************************************************/
static void print_optab_line(int* line)
{
    printf("%s ", tok2lex(line[0]));
    
    for(int j = 1; j < NENTS; j++)
    {
        printf("%5s ", tok2lex(line[j]));
    }
    printf("\n");
}

/**********************************************************************/
/* PUBLIC METHODS for this OBJECT  (EXPORTED)                        */
/**********************************************************************/
void p_optab()
{
    int num_rows = sizeof(optab) / sizeof(optab[0]);
    
    for(int i = 0; i < num_rows; i++)
    { 
        print_optab_line(optab[i]);
    }
}
/**********************************************************************/
/* return the type of a binary expression op arg1 arg2                */
/**********************************************************************/
toktyp get_otype(int op, toktyp arg1, toktyp arg2)
{
    int op_index       = 0;
    int arg1_index     = 1;
    int arg2_index     = 2;
    int res_type_index = 3;

    int num_rows = sizeof(optab) / sizeof(optab[0]);
    for(int i = 0; i < num_rows;i++)
    {
        int optab_op       = optab[i][op_index];
        int optab_arg1     = optab[i][arg1_index];
        int optab_arg2     = optab[i][arg2_index];

        if(op == optab_op && optab_arg1 == arg1 && optab_arg2 == arg2)
        {
            return optab[i][res_type_index];
        }
    }
    return optab[num_rows-1][res_type_index];
}

/**********************************************************************/
/* End of code                                                        */
/**********************************************************************/
