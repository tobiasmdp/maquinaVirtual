#include <stdio.h>
#include "parser.h"

int main()
{
  const char *lines[] = {
    "test: MOV AX, 123 ;inicializa AX",
    "\\\\STACK 5000",
    "BASE EQU #16",
    "TEXTO EQU \"Hola mundo!\""
  };

  const int size = sizeof(lines) / sizeof(char *);

  for (int i = 0; i < size; i++)
  {
    char **parsed = parseline(lines[i]);

    printf("  FULL LINE: %s\n", lines[i]);
    printf("      LABEL: %s\n", parsed[0] ? parsed[0] : "");
    printf("   MNEMONIC: %s\n", parsed[1] ? parsed[1] : "");
    printf("  OPERAND 1: %s\n", parsed[2] ? parsed[2] : "");
    printf("  OPERAND 2: %s\n", parsed[3] ? parsed[3] : "");
    printf("    COMMENT: %s\n", parsed[4] ? parsed[4] : "");
    printf("    SEGMENT: %s\n", parsed[5] ? parsed[5] : "");
    printf("  SEG. SIZE: %s\n", parsed[6] ? parsed[6] : "");
    printf(" CONST NAME: %s\n", parsed[7] ? parsed[7] : "");
    printf("CONST VALUE: %s\n", parsed[8] ? parsed[8] : "");
    printf("\n");

    freeline(parsed);
  }
}
