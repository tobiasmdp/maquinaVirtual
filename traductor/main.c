#include <stdio.h>
#include <string.h>
#include "parser.h"
#include "parser.c"

#define nombreArchB "traducido.mv1"
#define largoLinea 99

int main(int argc, char const *argv[])
{

/*   char **parsed = parseline("test: MOV AX, 123 ;inicializa AX"); // dise�ar todo en este main, a ir creando las clases y metodos
  // robar la forma en q se crean modulos,, de la libreria parser

  printf("    LABEL: %s\n", parsed[0] ? parsed[0] : "");
  printf(" MNEMONIC: %s\n", parsed[1] ? parsed[1] : "");
  printf("OPERAND 1: %s\n", parsed[2] ? parsed[2] : "");
  printf("OPERAND 2: %s\n", parsed[3] ? parsed[3] : "");
  printf("  COMMENT: %s\n", parsed[4] ? parsed[4] : "");

  freeline(parsed); 
  
  printf("%d \n",argc);
  for (int i = 0; argc >= i ; i++ ){
    printf("%s \n", argv[1]);
  }

  */



  //formato de llamada: .\main.exe "nombrearchivo.asm" -o (flag o para printear, es opcional)

  char nombreArchivo[100] = "test.asm"; //borrar la asignacion, es solo para debugging <------------------------------------
  char linea [largoLinea] = "";
  FILE * archT, *archB;

  strcpy(nombreArchivo, argv[0]);
  strcpy(nombreArchivo, "test.asm");

  if ((archT = fopen(nombreArchivo, "r")) != NULL ){
    
    archB = fopen(nombreArchB, "wb"); //creo el binario

    while (!feof(archT)){ //comienza el ciclo de lectura linea por linea
      fgets(linea, largoLinea, archT); //lee 1 linea de largo maximo 99
      printf("%s", linea);
    }
    
   
    fclose(archT);
  } //abro el archivo para lectura
  else
    printf("no encontre el archivo");


  

  //tiene que escribir un binario formato ".mv1"
  return 0;
}
