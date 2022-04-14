#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "parser.h"
#include "parser.c"

#define nombreArchB "traducido.mv1"
#define largoLinea 256

int mnemonico(char lineaParseada[]);
void toMayuscula(char* cadena);

int main(int argc, char *argv[])
{

    char nombreArchivo[100], linea[largoLinea], **lineaParseada;
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
            printf(" - COMMENT: %s\n", lineaParseada[4] ? lineaParseada[4] : "");


            
            instruccion = mnemonico(lineaParseada[1]);
            printf("     %02x %02x %02x %02x\n\n\n\n", (instruccion>>24)&0xFF, (instruccion>>16)&0xFF, (instruccion>>8)&0xFF, (instruccion)&0xFF);
            printf(" ");
        }
        fclose(archT);
    } // abro el archivo para lectura
    else
        printf("no encontre el archivo");

    // tiene que escribir un binario formato ".mv1"
    return 0;
}




int mnemonico(char lineaParseada[])
{
    int instruccion;

    toMayuscula(lineaParseada);

    if (strcmp(lineaParseada, "MOV") == 0)
        instruccion = 0x00000000;
    else if (strcmp(lineaParseada, "ADD") == 0)
        instruccion = 0x10000000;
    else if (strcmp(lineaParseada, "SUB") == 0)
        instruccion = 0x20000000;
    else if (strcmp(lineaParseada, "SWAP") == 0)
        instruccion = 0x30000000;
    else if (strcmp(lineaParseada, "MUL") == 0)
        instruccion = 0x40000000;
    else if (strcmp(lineaParseada, "DIV") == 0)
        instruccion = 0x50000000;
    else if (strcmp(lineaParseada, "CMP") == 0)
        instruccion = 0x60000000;
    else if (strcmp(lineaParseada, "SHL") == 0)
        instruccion = 0x70000000;
    else if (strcmp(lineaParseada, "SHR") == 0)
        instruccion = 0x80000000;
    else if (strcmp(lineaParseada, "AND") == 0)
        instruccion = 0x90000000;
    else if (strcmp(lineaParseada, "OR") == 0)
        instruccion = 0xA0000000;
    else if (strcmp(lineaParseada, "XOR") == 0)
        instruccion = 0xB0000000;
    // ---------------- 1 Operando ----------------
    else if (strcmp(lineaParseada, "SYS") == 0)
        instruccion = 0xF0000000;
    else if (strcmp(lineaParseada, "JMP") == 0)
        instruccion = 0xF1000000;
    else if (strcmp(lineaParseada, "JZ") == 0)
        instruccion = 0xF2000000;
    else if (strcmp(lineaParseada, "JP") == 0)
        instruccion = 0xF3000000;
    else if (strcmp(lineaParseada, "JN") == 0)
        instruccion = 0xF4000000;
    else if (strcmp(lineaParseada, "JNZ") == 0)
        instruccion = 0xF5000000;
    else if (strcmp(lineaParseada, "JNP") == 0)
        instruccion = 0xF6000000;
    else if (strcmp(lineaParseada, "JNN") == 0)
        instruccion = 0xF7000000;
    else if (strcmp(lineaParseada, "LDL") == 0)
        instruccion = 0xF8000000;
    else if (strcmp(lineaParseada, "LDH") == 0)
        instruccion = 0xF9000000;
    else if (strcmp(lineaParseada, "RND") == 0)
        instruccion = 0xFA000000;
    else if (strcmp(lineaParseada, "NOT") == 0)
        instruccion = 0xFB000000;
    // ---------------- 0 Operandos ----------------
    else if (strcmp(lineaParseada, "STOP") == 0)
        instruccion = 0xFF100000;
    // ---------------- Operando erroneo ----------------
    else
        instruccion = 0xF;
    return instruccion;
}

void toMayuscula(char* cadena){
    int i=0; 
    while (cadena[i]){
        cadena[i] = toupper(cadena[i]);
        i++;
    }
}