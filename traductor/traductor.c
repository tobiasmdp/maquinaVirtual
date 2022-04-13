#include <stdio.h>
#include <string.h>
#include "parser.h"
#include "parser.c"

#define nombreArchB "traducido.mv1"
#define largoLinea 256

int main(int argc, char *argv[])
{

  char nombreArchivo[100], linea[largoLinea], **lineaParseada, *mnemonico;
  FILE *archT, *archB;
  int instruccion = 0;

  strcpy(nombreArchivo, argv[1]); // arrancan desde el 1 los argumentos, 0 es el ejecutable

  if ((archT = fopen(nombreArchivo, "r")) != NULL)
  {

    archB = fopen(nombreArchB, "wb"); // creo el binario

    while (fgets(linea, largoLinea, archT) != NULL)
    { // comienza el ciclo de lectura linea por linea

      printf("%s", linea);

      lineaParseada = parseline(linea);
      printf(" - LABEL: %s", lineaParseada[0] ? lineaParseada[0] : "");
      printf(" - MNEMONIC: %s", lineaParseada[1] ? lineaParseada[1] : "");
      printf(" - OPERAND 1: %s", lineaParseada[2] ? lineaParseada[2] : "");
      printf(" - OPERAND 2: %s", lineaParseada[3] ? lineaParseada[3] : "");
      printf(" - COMMENT: %s\n\n\n", lineaParseada[4] ? lineaParseada[4] : "");
    }

    fclose(archT);
  } // abro el archivo para lectura
  else
    printf("no encontre el archivo");

  // tiene que escribir un binario formato ".mv1"
  return 0;
}
