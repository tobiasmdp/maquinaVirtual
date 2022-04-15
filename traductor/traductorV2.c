#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "parser.h"
#include "parser.c"

#define largoLinea 256
#define largoString 100
#define TOInmediato (0)
#define TORegistro (1)
#define TODirecto (2)

int getMnemonico(char* cadena);
int checkNumeric(char* cadena);
int checkInmediato(char* cadena);
int checkDirecto(char* cadena);
int checkRegistro(char* cadena);
int getTipoOperando(char* cadena);
void removeCorchetes(char* cadena, char* out);
int anyToInt(char *s, char **out);
int operandoRegistro(char *operandoEnString);
int getOperando(int tipoOperando, char* operandoEnString);
void getRotulos(char* nombreArchT);

int instruccion;
const char* tablaMnemonicos[3][16] = {{"","STOP","","","","","","","","","","","","","",""}, 
                            {"SYS","JMP","JZ","JP","JN","JNZ","JNP","JNN","LDL","LDH","RND","NOT","","","",""},
                            {"MOV","ADD","SUB","SWAP","MUL","DIV","CMP","SHL","SHR","AND","OR","XOR","","","",""}};
char* tablaRotulos[4096];

int main(int argc, char const *argv[]){
    FILE *archT, *archB;
    int mnemonico, operando1, operando2, tipoOperando1, tipoOperando2;
    char nombreArchT[largoString], nombreArchB[largoString],linea[largoLinea], **lineaParseada;

    //leerRotulosArchivo();

    strcpy(nombreArchT, argv[1]); // arrancan desde el 1 los argumentos, 0 es el ejecutable
    strcpy(nombreArchB, argv[2]); // arrancan desde el 1 los argumentos, 0 es el ejecutable

    getRotulos(nombreArchT);

    if ((archT = fopen(nombreArchT, "r")) != NULL){

        archB = fopen(nombreArchB, "wb"); // creo el binario

        while (fgets(linea, largoLinea, archT) != NULL){ // comienza el ciclo de lectura linea por linea
            lineaParseada = parseline(linea);
            /*
            LABEL: lineaParseada[0]
            MNEMONIC: lineaParseada[1]
            OPERAND 1: lineaParseada[2]
            OPERAND 2: lineaParseada[3]
            COMMENT:lineaParseada[4]
            */
            mnemonico = getMnemonico(lineaParseada[1]);
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
                tipoOperando1 = getTipoOperando(lineaParseada[2]);
                tipoOperando2 = getTipoOperando(lineaParseada[3]);
                instruccion |= tipoOperando1<<26;
                instruccion |= tipoOperando2<<24;
                operando1 = getOperando(tipoOperando1, lineaParseada[2]);
                operando2 = getOperando(tipoOperando2, lineaParseada[3]);
                instruccion |= (operando1<<12)&0x00FFF000;
                instruccion |= (operando2)&0x00000FFF;
            }
            else if (mnemonico >= 0xF0 && mnemonico <= 0xFB){ //1 OP
                instruccion = mnemonico<<24;
                tipoOperando1 = getTipoOperando(lineaParseada[2]);
                instruccion |= tipoOperando1<<22;
                operando1 = getOperando(tipoOperando1, lineaParseada[2]);
                instruccion |= (operando1)&0x0000FFFF;
            }
            else if(mnemonico >= 0xFF1 && mnemonico <= 0xFF1){ //2 OP
                instruccion = mnemonico<<20;
            }
            printf("     %02x %02x %02x %02x\n\n", (instruccion>>24)&0xFF, (instruccion>>16)&0xFF, (instruccion>>8)&0xFF, (instruccion)&0xFF);
            printf("a");

        }
        fclose(archT);
    }
    else
        printf("no encontre el archivo");

    return 0;
}


int getMnemonico(char* cadena){ //-1  no encontro el mnemonico
    for (int i=0; i<3; i++){
        for (int j=0; j<16; j++)
        if (stricmp(tablaMnemonicos[i][j], cadena) == 0){//i no importa mayusc
            if (i == 0) 
                return 0xFF<<4 |j;
            if (i == 1) 
                return 0xF<<4 |j;
            if (i == 2) 
                return j;
        } 
    }
    return -1;
}


int checkNumeric(char* cadena){ //se fija si es un numero valido || return 0 (false), return 1 (true) 
    int lenCadena = strlen(cadena);
    if (lenCadena == 0)
        return 0;
    for (int i=0; i < lenCadena; i++){
        if (cadena[i] < '0')
            return 0; 
        if ( cadena[i] > '9')
            return 0;
    }
    return 1; 
} 

int checkCaracter(char* cadena){ //mira si el string es solo de caracteres alfabeticos
    int i=0;
    while (cadena[i]){
        if (!isalpha(cadena[i]))
            return 0;
        i++;
    }
    return 1;
}

int checkInmediato(char* cadena){  
    if (cadena[0] == '%' || cadena[0] == '#' || cadena[0] == '@' || cadena[0] == '$')
        return 1;
    if (checkNumeric(cadena))
        return 1;
    if (checkCaracter(cadena)) //para el label, que se considera inmediato
        return 1;
    return 0;
}

int checkDirecto(char* cadena){
    if (cadena[0] == '[')
        return 1;
    return 0;
}

int checkRegistro(char* cadena){
    if ((cadena[0] >= 'A' && cadena[0] <= 'F') || (cadena[0] >= 'a' && cadena[0] <= 'f'))
        return 1;
    return 0;
}

int getTipoOperando(char* cadena){
    if (checkDirecto(cadena))
        return TODirecto;
    if (checkInmediato(cadena))
        return TOInmediato;
    if (checkRegistro(cadena))
        return TORegistro;
    return -1;
}

void removeCorchetes(char* cadena, char* out){ //en realidad se remueve el primer y ultimo char de la cadena
    int largoCadena = strlen(cadena);
    memcpy(out,cadena+1, largoCadena-2); //preferible copiar la data a corromper cadena
    out [largoCadena-2] = 0; //marco el fin de la cadena
}

int anyToInt(char *s, char **out){ //el out no se usa... se le pasa un cono
    const char* BASES = "**$*****@*#*****%";  ;
    int base = 10;
    char *bp = strchr(BASES, *s);
    if (bp != NULL){
        base = bp - BASES;
        ++s;
    }
    return strtol(s, out, base);
}

int operandoRegistro(char *operandoEnString){
    int largoCadena = strlen(operandoEnString);
    if (largoCadena == 2){ 
        if (toupper(operandoEnString[1]) == 'L') //toupper hace un return, no edita, hace uppercase
            return (1<<4 | toupper(operandoEnString[0])-55); //siempre la letra mayusc asi en el ascci siempre resto 55
        if (toupper(operandoEnString[1]) == 'H')
            return (2<<4 | toupper(operandoEnString[0])-55);
        if (toupper(operandoEnString[1]) == 'X')
            return (3<<4 | toupper(operandoEnString[0])-55);
    }
    if (largoCadena == 3){
        return (0 | toupper(operandoEnString[1])-55);
    }
}

int getOperando(int tipoOperando, char* operandoEnString){
    char operandoAux[64];
    char* cono; 
    if (tipoOperando == TORegistro){
        return operandoRegistro(operandoEnString);
    }
    if (tipoOperando == TODirecto){
        removeCorchetes(operandoEnString, operandoAux);
        return anyToInt(operandoAux, &cono);
    }
    if (tipoOperando == TOInmediato){
        if (checkCaracter(operandoEnString)) //si es label
            //busca el label, lo traduce etc..
        return anyToInt(operandoEnString, &cono); 
    }
}

void getRotulos(char* nombreArchT){
    int i=0;
    FILE *archT;
    char linea[largoLinea], **lineaParseada;
    if ((archT = fopen(nombreArchT, "r")) != NULL){
        while (fgets(linea, largoLinea, archT) != NULL){ 
            lineaParseada = parseline(linea);
            if (lineaParseada[0] != 0){
                tablaRotulos[i] = (char*) malloc(100);
                strcpy(tablaRotulos[i], lineaParseada[0]);
            }
            i++;
        }
    }


}


