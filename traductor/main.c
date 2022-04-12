#include <stdio.h>
#include "parser.h"
#include "parser.c"

int main(int argc, char const *argv[])
{
  char **parsed = parseline("test: MOV AX, 123 ;inicializa AX"); // dise�ar todo en este main, a ir creando las clases y metodos
  // robar la forma en q se crean modulos,, de la libreria parser

  printf("    LABEL: %s\n", parsed[0] ? parsed[0] : "");
  printf(" MNEMONIC: %s\n", parsed[1] ? parsed[1] : "");
  printf("OPERAND 1: %s\n", parsed[2] ? parsed[2] : "");
  printf("OPERAND 2: %s\n", parsed[3] ? parsed[3] : "");
  printf("  COMMENT: %s\n", parsed[4] ? parsed[4] : "");

  freeline(parsed);


  return 0;
}
