#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "parser.h"
#include "parser.c"

#define nombreArchB "traducido.mv1"
#define largoLinea 256
#define largoString 100
#define TOInmediato (0)
#define TORegistro (1)
#define TODirecto (2)
int instruccion;

int main(int argc, char *argv[])
{
    leerRotulosArchivo();


    int mnemonico, operando1, operando2, tipoOperando1, tipoOperando2;
    char nombreArchivo[largoString],

    strcpy(nombreArchivo, argv[1]); // arrancan desde el 1 los argumentos, 0 es el ejecutable
    if ((archT = fopen(nombreArchivo, "r")) != NULL)

//while que va de a una linea



    mnemonico = getMnemonico(_mnemonicoEnString);
    instruccion=0;
    if (mnemonico < 0){
        /*
         Error de sintaxis es el de una instrucción inexistente, en cuyo caso mostrará un mensaje
        indicándolo. La línea no será traducida, en la celda de la instrucción quedará en FF FF FF FF.
        */
       instruccion = 0xFFFFFFFF;
       printf("Error de Sintaxis");
    }
    else if (mnemonico >= 0x0 && mnemonico <= 0xB){ //2 OP
        instruccion = mnemonico<<28;
        tipoOperando1 = getTipoOperando(_operando1EnString);
        tipoOperando2 = getTipoOperando(_operando2EnString);
        instruccion |= tipoOperando1<<26;
        instruccion |= tipoOperando2<<24;
        operando1 = getOperando(tipoOperando1, _operando1EnString);
        operando1 = getOperando(tipoOperando1, _operando2EnString);
        instruccion |= (operando1<<12)&0x00FFF000;
        instruccion = (operando2)&0x00000FFF;
    }
    else if (mnemonico >= 0xF0 && mnemonico <= 0xFB){ //1 OP
        instruccion = mnemonico<<24;
        tipoOperando1 = getTipoOperando(_operando1EnString);
        instruccion |= tipoOperando1<<22;
        operando1 = getOperando(tipoOperando1, _operando1EnString);
        instruccion |= (operando1)&0x0000FFFF;
    }
    else if(mnemonico >= 0xFF1 && mnemonico <= 0xFF1){ //2 OP
        instruccion = mnemonico<<20;
    }
    return 0;
}




