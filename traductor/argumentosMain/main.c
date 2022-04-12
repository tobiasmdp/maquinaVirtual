#include <stdio.h>
#include "parser.h"
#include "parser.c"

int main(int argc, char const *argv[])
{

/*   char **parsed = parseline("test: MOV AX, 123 ;inicializa AX"); // dise�ar todo en este main, a ir creando las clases y metodos
  // robar la forma en q se crean modulos,, de la libreria parser

  printf("    LABEL: %s\n", parsed[0] ? parsed[0] : "");
  printf(" MNEMONIC: %s\n", parsed[1] ? parsed[1] : "");
  printf("OPERAND 1: %s\n", parsed[2] ? parsed[2] : "");
  printf("OPERAND 2: %s\n", parsed[3] ? parsed[3] : "");
  printf("  COMMENT: %s\n", parsed[4] ? parsed[4] : "");

  freeline(parsed); */

  printf("%d \n",argc); 
  //argc es la el largo del array argv +1, y argv se va a 
  //formar en cada posicion por la cadena de chars separada por espacio
  for (int i = 0; argc >= i ; i++ ){
    printf("%s \n", argv[i]);
  }
  //si le pasas pepe el grillo, es largo es 4 e imprime pepe, el, grillo

  return 0;
}
