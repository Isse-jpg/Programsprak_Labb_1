/**********************************************************************/
/* lab 1 DVG C01 - Symbol Table OBJECT                                */
/**********************************************************************/

/**********************************************************************/
/* Include files                                                      */
/**********************************************************************/
#include "symtab.h"
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
/**********************************************************************/
/* Other OBJECT's METHODS (IMPORTED)                                  */
/**********************************************************************/
#include "keytoktab.h"

/**********************************************************************/
/* OBJECT ATTRIBUTES FOR THIS OBJECT (C MODULE)                       */
/**********************************************************************/
#define TABSIZE 1024                   /* symbol table size           */
#define NAMELEN   20                   /* name length                 */

typedef char tname[NAMELEN];

static tname  name[TABSIZE];
static toktyp role[TABSIZE];
static toktyp type[TABSIZE];
static int    size[TABSIZE];
static int    addr[TABSIZE];

static int numrows=0;                  /* number of rows in the ST    */
static int startp =0;                  /* start position program in ST*/
static int next_free_addr = 0;         /* index of next free addr     */

/**********************************************************************/
/*  PRIVATE METHODS for this OBJECT  (using "static" in C)            */
/**********************************************************************/
/**********************************************************************/
/*  GET methods (one for each attribute)                              */
/**********************************************************************/
static char * get_name(int ftref)   { return name[ftref]; }
static toktyp get_role(int ftref)   { return role[ftref]; }
static toktyp get_type(int ftref)   { return type[ftref]; }
static int    get_size(int ftref)   { return size[ftref]; }
static int    get_addr(int ftref)   { return addr[ftref]; }

/**********************************************************************/
/*  SET methods (one for each attribute)                              */
/**********************************************************************/
static void set_name(int ftref, char * fpname) { strcpy(name[ftref],
                                                        fpname);}
static void set_role(int ftref, toktyp frole)  { role[ftref] = frole; }
static void set_type(int ftref, toktyp ftype)  { type[ftref] = ftype; }
static void set_size(int ftref, int    fsize)  { size[ftref] = fsize; }
static void set_addr(int ftref, int    faddr)  { addr[ftref] = faddr; }

/**********************************************************************/
/*  Add a row to the symbol table                                     */
/**********************************************************************/
static bool addrow(char *fname, toktyp frole, toktyp ftype,
                   int fsize, int faddr)
{
    if(find_name(fname) == true) return false;
    set_name(numrows, fname);
    set_role(numrows, frole);
    set_type(numrows, ftype);
    set_size(numrows, fsize);
    set_addr(numrows, faddr);
    numrows++;
    return true;

}
/**********************************************************************/
/*  Initialise the symbol table                                       */
/**********************************************************************/
static void initst()
{
    addrow(tok2lex(predef),  typ, predef, 0, 0);
    addrow(tok2lex(undef),   typ, predef, 0, 0);
    addrow(tok2lex(error),   typ, predef, 0, 0);
    addrow(tok2lex(integer), typ, predef, 4, 0);
    addrow(tok2lex(boolean), typ, predef, 4, 0);
    addrow(tok2lex(real),    typ, predef, 8, 0);
}
/**********************************************************************/
/*  return a reference to the ST (index) if name found else nfound    */
/**********************************************************************/
static int get_ref(char * fpname)
{
    for(int i = 0;i<numrows;i++)
    {
        if(strcmp(name[i], fpname) == 0)
        {
            return i;
        }
    }
    return nfound;
}

/**********************************************************************/
/*  PUBLIC METHODS for this OBJECT  (EXPORTED)                        */
/**********************************************************************/
/**********************************************************************/
/*  Display the symbol table                                          */
/**********************************************************************/
static void p_symrow(int ftref)
{
    printf("%11s %10s %10s %9d %9d \n", name[ftref], tok2lex(role[ftref]), tok2lex(type[ftref]), size[ftref], addr[ftref]);
}

void p_symtab()
{
    printf("________________________________________________________ \n");
    printf(" THE SYMBOL TABLE\n");
    printf("________________________________________________________ \n");
    printf("       NAME       ROLE       TYPE      SIZE      ADDR     \n");
    printf("________________________________________________________ \n");
    
    for(int i = startp; i < numrows; i++)
    {
        p_symrow(i);
    }
    
    printf("________________________________________________________ \n");
    printf(" STATIC STORAGE REQUIRED is %d BYTES\n", next_free_addr);
    printf("________________________________________________________ \n\n");
}

/**********************************************************************/
/*  Add a program name to the symbol table                            */
/**********************************************************************/
int find_name(char* fpname);
void addp_name(char * fpname)
{
    initst();
    startp = numrows;
    //if name is already in table
    addrow(fpname, program, program, 0, 0);
    
}

/**********************************************************************/
/*  Add a variable name to the symbol table                           */
/**********************************************************************/
void addv_name(char * fpname)
{
    addrow(fpname,var, undef, 0, 0);
    
}

/**********************************************************************/
/*  Find a name in the the symbol table                               */
/*  return a Boolean (true, false) if the name is in the ST           */
/**********************************************************************/
int find_name(char * fpname)
{
    for(int i = 0;i< numrows;i++)
    {
        if(strcmp(fpname, get_name(i)) == 0)
            return true;
    }
    return false;
}

/**********************************************************************/
/*  Set the type of an id list in the symbol table                    */
/**********************************************************************/
void setv_type(toktyp ftype)
{
    int template_index = get_ref(tok2lex(ftype));
    int type_size = get_size(template_index);

    for(int i = startp;i<numrows;i++)
    {
        if(get_type(i) == undef){
            set_size(i, type_size);
            set_type(i, ftype);
            set_addr(i, next_free_addr);

            next_free_addr += type_size;
        }
    }
    set_size(startp, next_free_addr);

}

/**********************************************************************/
/*  Get the type of a variable from the symbol table                  */
/**********************************************************************/
toktyp get_ntype(char * fpname)
{
    for(int i = 0;i< numrows;i++){
        if(strcmp(fpname, get_name(i)) == 0)
            return get_type(i);
    }
        return undef;
}

/**********************************************************************/
/* End of code                                                        */
/**********************************************************************/
