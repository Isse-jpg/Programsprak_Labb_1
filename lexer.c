/**********************************************************************/
/* lab 1 DVG C01 - Lexer OBJECT                                       */
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
#define BUFSIZE 1024
#define LEXSIZE   30
static char buffer[BUFSIZE];
static char lexbuf[LEXSIZE];
static int  pbuf  = 0;               /* current index program buffer  */
static int  plex  = 0;               /* current index lexeme  buffer  */

/**********************************************************************/
/*  PRIVATE METHODS for this OBJECT  (using "static" in C)            */
/**********************************************************************/
/**********************************************************************/
/* buffer functions                                                   */
/**********************************************************************/
/**********************************************************************/
/* Read the input file into the buffer                                */
/**********************************************************************/

static void get_prog()
{

    size_t bytes_read = fread(buffer, 1, sizeof(buffer) - 1, stdin);
    buffer[bytes_read] = '\0';
}

/**********************************************************************/
/* Display the buffer                                                 */
/**********************************************************************/

static void pbuffer()
{
    char* curr_char = &buffer[0];
    while(*curr_char != '\0')
    {
        printf("%c", *curr_char);
        curr_char++;

    }
    printf("\n");


}

/**********************************************************************/
/* Copy a character from the program buffer to the lexeme buffer      */
/**********************************************************************/

static char get_char()
{
    char c = buffer[pbuf];
    if(plex < LEXSIZE - 1)
    {
        lexbuf[plex] = c;
        plex++;
    }
    else
    {
     printf("LexBuf is full\n");
    }
    pbuf++;
    return c;
}

/**********************************************************************/
/* End of buffer handling functions                                   */
/**********************************************************************/

/**********************************************************************/
/*  PUBLIC METHODS for this OBJECT  (EXPORTED)                        */
/**********************************************************************/
/**********************************************************************/
/* Return a token                                                     */
/**********************************************************************/
int get_token()
{
    //för att kunna läsa in filen
    if(buffer[0] == '\0') 
    {
        get_prog();
        printf("________________________________________________________\n");
        printf("THE PROGRAM TEXT\n");
        printf("________________________________________________________\n");
        pbuffer();
    }

    plex = 0;
    while(isspace(buffer[pbuf])){
        pbuf++;
    }

    if(buffer[pbuf] == '\0')
    {
        lexbuf[0] = '$';
        lexbuf[1] = '\0';
        return lex2tok(lexbuf); 
    }
    const char* symbols = "$()*+,-./:;=";

    while(buffer[pbuf] != '\0' && !isspace(buffer[pbuf])) 
    {
        char curr_char = buffer[pbuf];
        if (plex == 0 && curr_char == ':' && buffer[pbuf + 1] == '=') 
        {
            get_char(); 
            get_char();            
            break;
        }
        if(strchr(symbols,curr_char) != NULL){
            if(plex != 0){
                break;
            }else{
                get_char();
                break;
            }
        }
        get_char();

    }
    lexbuf[plex] = '\0';
    toktyp tok = lex2tok(lexbuf);
    return tok;
}

/**********************************************************************/
/* Return a lexeme                                                    */
/**********************************************************************/
char * get_lexeme()
{
    return lexbuf; 
}

/**********************************************************************/
/* End of code                                                        */
/**********************************************************************/
