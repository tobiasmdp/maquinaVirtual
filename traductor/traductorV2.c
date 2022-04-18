#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "parser.h"
#include "parser.c"

#define largoLinea 256
#define largoString 100
#define largoMemoria 4096
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
void getTablaRotulos(char* nombreArchT);
int getRotulo(char* operandoEnString);
int checkSalto(int mnemonico);
int checkRotulo(char* operandoEnString, int mnemonico);
void transformRotulo(char* operandoEnString, int mnemonico);
void checkTruncado(int operando, int bits);
void printeo(int dirMem, int instruccion, char* lineaParseada[]);
void getHeader(int cantCeldas);

int instruccion, tablaInstrucciones[largoMemoria], header[6], exito=1; //exito significa 0 errores
const char* tablaMnemonicos[3][16] = {{"","STOP","","","","","","","","","","","","","",""}, 
                            {"SYS","JMP","JZ","JP","JN","JNZ","JNP","JNN","LDL","LDH","RND","NOT","","","",""},
                            {"MOV","ADD","SUB","SWAP","MUL","DIV","CMP","SHL","SHR","AND","OR","XOR","","","",""}};
char* tablaRotulos[largoMemoria];


int main(int argc, char const *argv[]){
    FILE *archT, *archB;
    int mnemonico, operando1, operando2, tipoOperando1, tipoOperando2, dirMem=0, cantCeldas;
    char nombreArchT[largoString], nombreArchB[largoString],linea[largoLinea], **lineaParseada, * lineaParseadaOriginal[5];

    strcpy(nombreArchT, argv[1]); // arrancan desde el 1 los argumentos, 0 es el ejecutable
    strcpy(nombreArchB, argv[2]); // arrancan desde el 1 los argumentos, 0 es el ejecutable

    getTablaRotulos(nombreArchT);

    if ((archT = fopen(nombreArchT, "r")) != NULL){

        while (fgets(linea, largoLinea, archT) != NULL){ // comienza el ciclo de lectura linea por linea

            lineaParseada = parseline(linea);
            
            if ((lineaParseada[1] != 0)){ //si la linea se considera instruccion
                instruccion=0;

                for( int j = 0; j < 5; j++ ) lineaParseadaOriginal[j] = strdup(lineaParseada[j]); //copia el array de strings
        
                mnemonico = getMnemonico(lineaParseada[1]);
                if (mnemonico < 0){
                    instruccion = 0xFFFFFFFF;
                    printf("Error de Sintaxis");
                    exito=0;
                }
                else if (mnemonico >= 0x0 && mnemonico <= 0xB){ //2 OP
                    instruccion = mnemonico<<28;
                    tipoOperando1 = getTipoOperando(lineaParseada[2]);
                    tipoOperando2 = getTipoOperando(lineaParseada[3]);
                    instruccion |= tipoOperando1<<26;
                    instruccion |= tipoOperando2<<24;
                    operando1 = getOperando(tipoOperando1, lineaParseada[2]);
                    operando2 = getOperando(tipoOperando2, lineaParseada[3]);
                    checkTruncado(operando1,12);
                    checkTruncado(operando2,12);
                    instruccion |= (operando1<<12)&0x00FFF000;
                    instruccion |= (operando2)&0x00000FFF;
                }
                else if (mnemonico >= 0xF0 && mnemonico <= 0xFB){ //1 OP
                    transformRotulo(lineaParseada[2], mnemonico);
                    instruccion = mnemonico<<24;
                    tipoOperando1 = getTipoOperando(lineaParseada[2]);
                    instruccion |= tipoOperando1<<22;
                    operando1 = getOperando(tipoOperando1, lineaParseada[2]);
                    checkTruncado(operando1,12);
                    instruccion |= (operando1)&0x0000FFFF;
                }
                else if(mnemonico >= 0xFF1 && mnemonico <= 0xFF1){ //0 OP
                    instruccion = mnemonico<<20;
                }
                tablaInstrucciones[dirMem] = instruccion; 

                if(argc <4 || (strcmp(argv[3],"-o") != 0))
                    printeo(dirMem, instruccion, lineaParseadaOriginal);
                
                dirMem++;
            }
            else //se considera una linea q vacia o de comentario
                if(lineaParseada[4] != 0)
                    printf("\n%s\n",lineaParseada[4]);

            freeline(lineaParseada);
        }
        getHeader(dirMem); //dirMem+1 = cantCeldas  
        fclose(archT);
    }
    else
        printf("no se encontro el archivo");

    if(exito)
        if ((archB = fopen(nombreArchB, "wb")) != NULL){
            fwrite(header, sizeof(int), 6, archB);
            fwrite(tablaInstrucciones, sizeof(int), dirMem, archB);
            fclose(archB); 
        }

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

    if (cadena[0] == '%' || cadena[0] == '#' || cadena[0] == '@' || cadena[0] == '$' || strncmp(cadena,"‘",3) == 0) //la comilla puede ser confundida
        return 1;
    if (checkNumeric(cadena))
        return 1;
    return 0;
}

int checkDirecto(char* cadena){
    if (cadena[0] == '[')
        return 1;
    return 0;
}

int checkRegistro(char* cadena){
    int largoCadena = strlen(cadena);
    if (largoCadena == 2)
        if ((cadena[0] >= 'A' && cadena[0] <= 'F') || (cadena[0] >= 'a' && cadena[0] <= 'f'))
            return 1;
    if (largoCadena == 3)
        if (cadena[0] >= 'E'
        && ((cadena[1] >= 'A' && cadena[1] <= 'F') || (cadena[1] >= 'a' && cadena[1] <= 'f')) 
        && (cadena[2] == 'X' || cadena[2] == 'H' || cadena[2] == 'L'))
            return 1;
    return 0;
}

int getTipoOperando(char* cadena){
    if (checkDirecto(cadena))
        return TODirecto;
    if (checkRegistro(cadena))
        return TORegistro;
    if (checkInmediato(cadena))
        return TOInmediato;
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


/* 
https://stackoverflow.com/questions/59838304/how-can-i-change-an-apostrophe-into-a-single-unit-instead-of-3-bytes-342-200
el apostrofe ocupa 3 bytes
*/
int getOperando(int tipoOperando, char* operandoEnString){
    char operandoAux[64];
    char* cono; 
    if (tipoOperando == TORegistro){
        return operandoRegistro(operandoEnString);
    }
    if (tipoOperando == TODirecto){
        removeCorchetes(operandoEnString, operandoAux);
        if (strncmp(operandoAux,"‘",3) == 0)
            return (int)operandoAux[3];
        return anyToInt(operandoAux, &cono);
    }
    if (tipoOperando == TOInmediato){
        if (strncmp(operandoEnString,"‘",3) == 0)
            return (int)operandoEnString[3];
        return anyToInt(operandoEnString, &cono); 
    }
}

void getTablaRotulos(char* nombreArchT){
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

int getRotulo(char* operandoEnString){
     for( int i=0; i<largoLinea; i++)
        if(stricmp(operandoEnString, tablaRotulos[i]) == 0)
            return i; //i es la pos de memoria corresp a ese Rotulo
    return -1;
}

int checkSalto(int mnemonico){
    if (mnemonico >= 0xf1 && mnemonico <= 0xf7)
        return 1;
    return 0;
}

int checkRotulo(char* operandoEnString, int mnemonico){
    if (! checkRegistro(operandoEnString)){ 
        if (checkSalto(mnemonico))
            return 1;
    }
    return 0;
}

void transformRotulo(char* operandoEnString, int mnemonico){
    int rotuloInt = getRotulo(operandoEnString);
    char aux[largoString];
    if (checkRotulo(operandoEnString, mnemonico)){
        if (rotuloInt != -1)
            sprintf(aux, "%d", rotuloInt);
        else{
            sprintf(aux, "%X", 0xFFF);
            printf("Error, No se encuentra el rótulo");
            exito=0;
        }
        strcpy(operandoEnString, aux);
    }
}

void checkTruncado(int operando, int bits){
    if (bits == 16)
        if ((operando & 0xFFFF0000) != 0){
            printf("Truncado de Operando: \n%d no puede ser almacenado en el espacio de %d bits. \nSe trunca la parte alta del valor.\n", operando, bits);
        }
    if (bits == 12)
                if ((operando & 0xFFFFF000) != 0){
            printf("Truncado de Operando: \n%d no puede ser almacenado en el espacio de %d bits. \nSe trunca la parte alta del valor.\n", operando, bits);
        }
}

void printeo(int dirMem, int instruccion, char* lineaParseada[]){
    char coma[largoLinea]=";";
    if(lineaParseada[1]){ //tiene mnemonico --> no es comentario
        if (lineaParseada[0] != 0) //si tiene rotulo
            printf("[%04d]:  %02X %02X %02X %02X %11s: %4s %7s %-11s %s\n\n", dirMem, (instruccion>>24)&0xFF, (instruccion>>16)&0xFF, (instruccion>>8)&0xFF, (instruccion)&0xFF, 
            lineaParseada[0], lineaParseada[1], (lineaParseada[2] == 0) ? "" : lineaParseada[2],
            (lineaParseada[3] == 0) ? "" : lineaParseada[3], (lineaParseada[4] == 0) ? "" : strcat(coma,lineaParseada[4]));
        else
            printf("[%04d]:  %02X %02X %02X %02X %11d: %4s %7s %-11s %s\n\n", dirMem, (instruccion>>24)&0xFF, (instruccion>>16)&0xFF, (instruccion>>8)&0xFF, (instruccion)&0xFF, 
            dirMem+1, lineaParseada[1], (lineaParseada[2] == 0) ? "" : lineaParseada[2], 
            (lineaParseada[3] == 0) ? "" : lineaParseada[3], (lineaParseada[4] == 0) ? "" : strcat(coma,lineaParseada[4]));
    }
    else{
        printf(lineaParseada[4]);
    }
}

void getHeader(int cantCeldas){
    header[0]=((int)'M'<<24 | (int)'V'<<16 | (int)'-'<<8 | (int)'1');
    header[1]=cantCeldas;
    header[2]=0;
    header[3]=0;
    header[4]=0;
    header[5]=((int)'V'<<24 | (int)'.'<<16 | (int)'2'<<8 | (int)'2');
}