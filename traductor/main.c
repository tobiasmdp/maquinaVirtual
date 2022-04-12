#include <stdio.h>
#include <string.h>
#include "parser.h"
#include "parser.c"

#define nombreArchB "traducido.mv1"
#define largoLinea 99

int main(int argc, char const *argv[])
{

/*   char **lineaParseada = parseline("test: MOV AX, 123 ;inicializa AX"); // dise�ar todo en este main, a ir creando las clases y metodos
  // robar la forma en q se crean modulos,, de la libreria parser

  printf("    LABEL: %s\n", lineaParseada[0] ? lineaParseada[0] : "");
  printf(" MNEMONIC: %s\n", lineaParseada[1] ? lineaParseada[1] : "");
  printf("OPERAND 1: %s\n", lineaParseada[2] ? lineaParseada[2] : "");
  printf("OPERAND 2: %s\n", lineaParseada[3] ? lineaParseada[3] : "");
  printf("  COMMENT: %s\n", lineaParseada[4] ? lineaParseada[4] : "");

  freeline(lineaParseada); 
  
  printf("%d \n",argc);
  for (int i = 0; argc >= i ; i++ ){
    printf("%s \n", argv[1]);
  }

  */



  //formato de llamada: .\main.exe "nombrearchivo.asm" -o (flag o para printear, es opcional)

  char nombreArchivo[100], linea [largoLinea], ** lineaParseada;

  FILE * archT, *archB;

  strcpy(nombreArchivo, argv[0]);
  strcpy(nombreArchivo, "test.asm"); //borrar la asignacion, es solo para debugging <------------------------------------

  if ((archT = fopen(nombreArchivo, "r")) != NULL ){
    
    archB = fopen(nombreArchB, "wb"); //creo el binario

    while (!feof(archT)){ //comienza el ciclo de lectura linea por linea
      fgets(linea, largoLinea, archT); //lee 1 linea de largo maximo 99
      printf("%s", linea);

      lineaParseada = parseline(linea);
      printf(" - LABEL: %s", lineaParseada[0] ? lineaParseada[0] : "");
      printf(" - MNEMONIC: %s", lineaParseada[1] ? lineaParseada[1] : "");
      printf(" - OPERAND 1: %s", lineaParseada[2] ? lineaParseada[2] : "");
      printf(" - OPERAND 2: %s", lineaParseada[3] ? lineaParseada[3] : "");
      printf(" - COMMENT: %s\n\n\n", lineaParseada[4] ? lineaParseada[4] : "");

    }
    


   
    fclose(archT);
  } //abro el archivo para lectura
  else
    printf("no encontre el archivo");


  

  //tiene que escribir un binario formato ".mv1"
  return 0;
}
