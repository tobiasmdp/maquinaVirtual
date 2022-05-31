#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "parser.h"
#include "parser.c"

#define largoLinea 256
#define largoString 100
#define largoMemoria 8192
#define largoParser 8
#define largoSimbolo 11
#define largoRegistro 5
#define TOInmediato 0
#define TORegistro 1
#define TODirecto 2
#define TOIndirecto 3

typedef struct simbolo
{
    int tipo; /*tipo=0 rotulo, tipo=1 cte num, tipo=2 cte string */
    char *label;
    char *contenido; /*en contenido esta el string si es string, el entero si es entero, y la pos si es label*/
    int posCS; /*nunca puede ser 0 porque siempre va a estar despues de las instrucciones, si es -1 es que algo anda mal*/
} simbolo; /*para mas comodidad se usan todos con mayusculas*/

void setTablaCteString();
int getMnemonico(char* cadena);
int checkNumeric(char* cadena);
int checkInmediato(char* cadena);
int checkDirecto(char* cadena);
int checkRegistro(char* cadena);
int getTipoOperando(char* cadena);
void removeExtremos(char* cadena, char* out);
int anyToInt(char *s, char **out);
int operandoRegistro(char *operandoEnString);
int getOperando(int tipoOperando, char* operandoEnString);
void getTablaSimbolos(char* nombreArchT);
simbolo getSimbolos(char* operandoEnString);
int checkSalto(int mnemonico);
int checkSimbolo(char* simbolo);
int transformSimbolo(char* cadena);
void checkTruncado(int operando, int bits);
void printeo(int dirMem, int instruccion, char* lineaParseada[]);
void getHeader();
int updateSegmento(char* segmento, char* tamanio);
void upcaseString (char str[],char final[]);
int checkFormatoSimbolo(char* cadena);
void addCorchetes(char* cadena, char* out);
void transformNull(char* cadena);

int instruccion, tablaInstrucciones[largoMemoria], header[6], exito=1, CS, DS=1024, SS=1024, ES=1024, tempCS=0, cantSimbolos=0, cantString=0; //exito significa 0 errores
const char* tablaMnemonicos[3][16] = {{"RET","STOP","","","","","","","","","","","","","",""}, 
                            {"SYS","JMP","JZ","JP","JN","JNZ","JNP","JNN","LDL","LDH","RND","NOT","PUSH","POP","CALL",""},
                            {"MOV","ADD","SUB","SWAP","MUL","DIV","CMP","SHL","SHR","AND","OR","XOR","SLEN","SMOV","SCMP",""}};
int tablaString[800];
simbolo tablaSimbolos[largoMemoria];

    // "      LABEL: %s\n", parsed[0] 
    // "   MNEMONIC: %s\n", parsed[1] 
    // "  OPERAND 1: %s\n", parsed[2] 
    // "  OPERAND 2: %s\n", parsed[3] 
    // "    COMMENT: %s\n", parsed[4] 
    // "    SEGMENT: %s\n", parsed[5] 
    // "  SEG. SIZE: %s\n", parsed[6] 
    // " CONST NAME: %s\n", parsed[7] 
    // "CONST VALUE: %s\n", parsed[8] 

int main(int argc, char const *argv[]){
    FILE *archT, *archB;
    int mnemonico, operando1, operando2, tipoOperando1, tipoOperando2, dirMem=0;
    char nombreArchT[largoString], nombreArchB[largoString],linea[largoLinea], **lineaParseada, * lineaParseadaOriginal[largoParser], *cono;

    // se usa la lineaParseadaOriginal por el mnemonico, que se transforma en un int hacia la instruccion correspondiente, 
    // pero luego al momento de mostrar por pantalla, se requiere el mnemonico original, no el numero de celda al cual va a saltar

    strcpy(nombreArchT, argv[1]); // arrancan desde el 1 los argumentos, 0 es el ejecutable
    strcpy(nombreArchB, argv[2]); // arrancan desde el 1 los argumentos, 0 es el ejecutable

    getTablaSimbolos(nombreArchT); //genera tabla de rotulos paralela
    setTablaCteString();// crear vector de las constantes en binario y finalizar el valor del cs
    
    if ((archT = fopen(nombreArchT, "r")) != NULL){

        while (fgets(linea, largoLinea, archT) != NULL){ // comienza el ciclo de lectura linea por linea

            lineaParseada = parseline(linea); // parser catedra

            if ((lineaParseada[1] != 0)){ //si la linea se considera instruccion con mnemmonicco
                instruccion=0;

                for( int j = 0; j < largoParser; j++ ) lineaParseadaOriginal[j] = strdup(lineaParseada[j]); //copia el array de strings
        
                mnemonico = getMnemonico(lineaParseada[1]);
                if (mnemonico < 0){ //validacion error mnemonico
                    instruccion = 0xFFFFFFFF;
                    printf("Error de Sintaxis, mnemonico inexistente");
                    printf("\n\n");
                    exito = 0;
                }
                else if (mnemonico >= 0x0 && mnemonico <= 0xF){ //2 OP
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
                else if (mnemonico >= 0xF0 && mnemonico <= 0xFF){ //1 OP 
                    instruccion = mnemonico<<24;
                    tipoOperando1 = getTipoOperando(lineaParseada[2]);
                    instruccion |= (tipoOperando1)<<22;
                    operando1 = getOperando(tipoOperando1, lineaParseada[2]);
                    operando1<<16;
                    operando1>>16;
                    checkTruncado(operando1,16);
                    instruccion |= (operando1)&0x0000FFFF;
                }
                else if(mnemonico >= 0xFF0 && mnemonico <= 0xFFF){ //0 OP
                    instruccion = mnemonico<<20;
                }
                tablaInstrucciones[dirMem] = instruccion; 

                if(argc <4 || (strcmp(argv[3],"-o") != 0))
                    printeo(dirMem, instruccion, lineaParseadaOriginal);
                
                dirMem++;
            }
            else if (lineaParseada[5] != 0){ //asignacion de segmento de memoria            
                long long int aux  = atoll(lineaParseada[6]);
                if ( 2147483647 < aux || -2147483647 > aux){
                    printf("Valores inapropiado en directiva");
                    printf("\n\n");
                    instruccion = 0xFFFFFFFF;
                    exito = 0;
                }
                else{
                    updateSegmento(lineaParseada[5], lineaParseada[6]);
                    if(argc <4 || (strcmp(argv[3],"-o") != 0))
                        printeo(dirMem, instruccion, lineaParseada);
                }
            }  
            else if(lineaParseada[4] != 0 || lineaParseada[7] != 0){ //linea comentario
                if(argc <4 || (strcmp(argv[3],"-o") != 0))
                    printeo(dirMem, instruccion, lineaParseada);
            }
                
            freeline(lineaParseada);
        }
        getHeader(); 
        fclose(archT);

        if(exito) //si algo sale mal, la var global exito se torna 0 (false)
            if ((archB = fopen(nombreArchB, "wb")) != NULL){
                fwrite(header, sizeof(int), 6, archB);
                fwrite(tablaInstrucciones, sizeof(int), dirMem, archB);//ver el dirMem aca
                fwrite(tablaString,sizeof(int),cantString,archB);
                fclose(archB); 
            }
    }
    else
        printf("no se encontro el archivo");
    return 0;
}

void setTablaCteString(){
int cont=0;
    for (int i=0;i<cantSimbolos;i++){
        if (tablaSimbolos[i].tipo==2){
            tablaSimbolos[i].posCS=tempCS+cont;
            for(int j=0;j<strlen(tablaSimbolos[i].contenido);j++){
                tablaString[cont]=tablaSimbolos[i].contenido[j];
                cont++;
            }
            tablaString[cont]=00;
            cont++;
        }
        else
        tablaSimbolos[i].posCS=-1;         
    }
    CS=tempCS+cont;
}

int getMnemonico(char* cadena){ 
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
    return -1; //-1  no encontro el mnemonico
}

int checkNumeric(char* cadena){ //se fija si es un numero valido
    int lenCadena = strlen(cadena), i=0;
    if (lenCadena == 0)
        return 0;
    if (cadena[0]=='-')
       i++;
    for (i; i < lenCadena; i++){
        if (cadena[i] < '0')
            return 0; 
        if ( cadena[i] > '9')
            return 0;
    }
    return 1; 
} 

int checkStringAlfa(char* cadena){ //mira si el string es solo de caracteres alfabeticos
    int i=0; //pepe
    while (cadena[i]){
        if (!isalpha(cadena[i]))
            return 0;
        i++;
    }
    return 1;
}

int checkFormatoSimbolo(char* cadena){//check if cumple con formato de un simbolo
    char cadenaAux[largoSimbolo];
    int contieneRegistro = 0, largoCadena = strlen(cadena); 
    if (largoCadena <= 15 && largoCadena >= 3 && isalpha(cadena[0])){
        memcpy(cadenaAux, cadena, 2);
        cadenaAux[2] = '\0';
        if (checkRegistro(cadenaAux)) // check si contiene resgistro largo 2
            contieneRegistro = 2; //asigno el largo del registro
        else{
            memcpy(cadenaAux, cadena, 3);
            cadenaAux[3] = '\0';
            if (checkRegistro(cadenaAux)) // check si contiene resgistro largo 3
                contieneRegistro = 3;
        }
        if (contieneRegistro){ //si contiene registro
            if (largoCadena == contieneRegistro) //si es solo un registro
                return 0;
            if (cadena[contieneRegistro] == '+' || cadena[contieneRegistro] == '-'){ //si le sigue un + o - al registro
                return 0;
            }
        }
        return 1;
    }
    return 0;
}

int checkInmediato(char* cadena){  
    if (cadena[0] == '%' || cadena[0] == '#' || cadena[0] == '@' || cadena[0] == '\'' || cadena[0] == '$')
        return 1;
    if (checkNumeric(cadena)) 
        return 1;
    if (checkFormatoSimbolo(cadena)) //requerimientos del formato de un simbolo
        if (transformSimbolo(cadena))
            return 2;
    return 0;
}

int checkDirecto(char* cadena){ //tienen corchetes y contienen un inmediato o cte-
    char cadenaAux[10];
    int resultado=0;
    if (cadena[0] == '['){ //miro el corchete
        removeExtremos(cadena, cadenaAux);
        resultado = checkInmediato(cadenaAux); //miro si es inmediato
        if (resultado == 2) //si resultado es 2, es porque era una constante 
            strcpy(cadena,cadenaAux); //modifico el vector de la linea parseada
    }
    return resultado;
}

int checkRegistro(char* cadena){
    int largoCadena = strlen(cadena), i;
    char cadenaAux[5];
    if (strlen(cadena) <= largoRegistro){
        strcpy(cadenaAux, cadena);
        if (stricmp(cadenaAux,"DS") == 0 || stricmp(cadenaAux,"SS") == 0 || stricmp(cadenaAux,"ES") == 0 || stricmp(cadenaAux,"CS") == 0 || stricmp(cadenaAux,"HP") == 0 || stricmp(cadenaAux,"IP") == 0 || stricmp(cadenaAux,"SP") == 0 || stricmp(cadenaAux,"BP") == 0 || stricmp(cadenaAux,"CC") == 0 || stricmp(cadenaAux,"AC") == 0)
            return 1; // no es registro de proposito general
        for (i = 0; i < largoCadena; i++) // mayusc copia de cadena
            cadenaAux[i] = toupper(cadena[i]);
        cadenaAux[i]= '\0';
        if (largoCadena == 2)
            if (cadenaAux[0] >= 'A' && cadenaAux[0] <= 'F' && (cadenaAux[1] == 'X' || cadenaAux[1] == 'H' || cadenaAux[1] == 'L'))
                return 1;
        if (largoCadena == 3)
            if (cadenaAux[0] == 'E' && (cadenaAux[1] >= 'A' && cadenaAux[1] <= 'F') && cadenaAux[2] == 'X')
                return 1;
    }
    return 0;
}

int checkIndirecto(char* cadena){ //tiene corchetes y un registro + opcional un offset
    int resultado = 0, largoCadenaAux, largoRegistroAux = 0;
    char cadenaAux[20], registroAux[4], offsetAux[largoSimbolo];
    if (cadena[0] == '[') { // miro el corchete
        removeExtremos(cadena, cadenaAux); 
        largoCadenaAux = strlen(cadenaAux);
        if (largoCadenaAux == 2 || largoCadenaAux == 3){//registro largo 2  || registro largo 3
            if (checkRegistro(cadenaAux)){
                resultado = 1; 
                strcpy(cadena, cadenaAux);
            }     
        }
        else if (largoCadenaAux >= 3){
            if (cadenaAux[2] == '+' || cadenaAux[2] == '-')
                largoRegistroAux = 2; 
            else if (cadenaAux[3] == '+' || cadenaAux[3] == '-')
                largoRegistroAux = 3;
            if (cadenaAux[largoRegistroAux] == '+' || cadenaAux[largoRegistroAux] == '-'){ //miro si existe un offset
                memcpy(offsetAux, cadenaAux+largoRegistroAux+1, largoCadenaAux-largoRegistroAux-1);
                offsetAux[largoCadenaAux-largoRegistroAux-1] = '\0';
                if (checkInmediato(offsetAux)){
                    resultado=1;
                    strcpy(cadenaAux+largoRegistroAux+1, offsetAux);
                    strcpy(cadena, cadenaAux);
                }
            }
        }   
    }
    return resultado;
}

int getTipoOperando(char* cadena){
    if (instruccion != -1 && checkDirecto(cadena))
        return TODirecto;
    if (instruccion != -1 && checkRegistro(cadena))
        return TORegistro;
    if (instruccion != -1 && checkInmediato(cadena))
        return TOInmediato;
    if (instruccion != -1 && checkIndirecto(cadena))
        return TOIndirecto;
    return -1; //operando invalido
}

void removeExtremos(char* cadena, char* out){ //remueve el primer y ultimo char de la cadena
    int largoCadena = strlen(cadena);
    memcpy(out,cadena+1, largoCadena-2); //preferible copiar la data a corromper cadena
    out [largoCadena-2] ='\0'; //marco el fin de la cadena
}

int anyToInt(char *s, char **out){ //el out no se usa... se le pasa un cono
    if (s[0]=='\'')
        return s[1];
    else{
        const char* BASES = "**$*****@*#*****%";  ;
        int base = 10;
        char *bp = strchr(BASES, *s);
        if (bp != NULL){
            base = bp - BASES;
            ++s;
        }
        return strtol(s, out, base);
    }
}

int operandoRegistro(char *operandoEnString){
    int largoCadena = strlen(operandoEnString);
    // Registros especificos
    if (stricmp(operandoEnString, "DS") == 0)
        return (0);
    if (stricmp(operandoEnString, "SS") == 0)
        return (1);
    if (stricmp(operandoEnString, "ES") == 0)
        return (2);
    if (stricmp(operandoEnString, "CS") == 0)
        return (3);
    if (stricmp(operandoEnString, "HP") == 0)
        return (4);
    if (stricmp(operandoEnString, "IP") == 0)
        return (5);
    if (stricmp(operandoEnString, "SP") == 0)
        return (6);
    if (stricmp(operandoEnString, "BP") == 0)
        return (7);
    if (stricmp(operandoEnString, "CC") == 0)
        return (8);    
    if (stricmp(operandoEnString, "AC") == 0)
        return (9);
    // Registros de proposito general
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

int operandoIndirecto(char* operandoEnString){
    int resultado = 0, largoCadenaAux, largoRegistroAux = 0, offset;
    char cadenaAux[20], registroAux[4], offsetAux[largoSimbolo], *cono; 
    registroAux[0] = '0'; 
    simbolo rotuloAux; 
    strcpy(cadenaAux, operandoEnString);
    largoCadenaAux = strlen(cadenaAux);
    if (largoCadenaAux == 2){// if contien registro largo 2
        if (stricmp(operandoEnString, "DS") == 0)
            resultado = (0);
        else if (stricmp(operandoEnString, "SS") == 0)
            resultado = (1);
        else if (stricmp(operandoEnString, "ES") == 0)
            resultado = (2);
        else if (stricmp(operandoEnString, "CS") == 0)
            resultado = (3);
        else if (stricmp(operandoEnString, "HP") == 0)
            resultado = (4);
        else if (stricmp(operandoEnString, "IP") == 0)
            resultado = (5);
        else if (stricmp(operandoEnString, "SP") == 0)
            resultado = (6);
        else if (stricmp(operandoEnString, "BP") == 0)
            resultado = (7);
        else if (stricmp(operandoEnString, "CC") == 0)
            resultado = (8);    
        else if (stricmp(operandoEnString, "AC") == 0)
            resultado = (9);
        else
            resultado = toupper(cadenaAux[0])-55; 
    }
    else if (largoCadenaAux == 3)//contiene registro largo 3
        resultado = toupper(cadenaAux[1])-55; 
    else{
        if (cadenaAux[2] == '+' || cadenaAux[2] == '-'){
            if (toupper(cadenaAux[0]) == 'D' && toupper(cadenaAux[1]) == 'S')
                resultado = (0);
            else if (toupper(cadenaAux[0]) == 'S' && toupper(cadenaAux[1]) == 'S')
                resultado = (1);
            else if (toupper(cadenaAux[0]) == 'E' && toupper(cadenaAux[1]) == 'S')
                resultado = (2);
            else if (toupper(cadenaAux[0]) == 'C' && toupper(cadenaAux[1]) == 'S')
                resultado = (3);
            else if (toupper(cadenaAux[0]) == 'H' && toupper(cadenaAux[1]) == 'P')
                resultado = (4);
            else if (toupper(cadenaAux[0]) == 'I' && toupper(cadenaAux[1]) == 'P')
                resultado = (5);
            else if (toupper(cadenaAux[0]) == 'S' && toupper(cadenaAux[1]) == 'P')
                resultado = (6);
            else if (toupper(cadenaAux[0]) == 'B' && toupper(cadenaAux[1]) == 'P')
                resultado = (7);
            else if (toupper(cadenaAux[0]) == 'C' && toupper(cadenaAux[1]) == 'C')
                resultado = (8);
            else if (toupper(cadenaAux[0]) == 'A' && toupper(cadenaAux[1]) == 'C')
                resultado = (9);
            else
                resultado = toupper(cadenaAux[0])-55; 
            largoRegistroAux = 2;
        }
        else{   
            resultado = toupper(cadenaAux[1])-55;
            largoRegistroAux = 3;
        }
        memcpy(offsetAux, cadenaAux+largoRegistroAux+1, largoCadenaAux-largoRegistroAux-1);
        offsetAux[largoCadenaAux-largoRegistroAux-1] = '\0';
        offset = anyToInt(offsetAux,&cono);
        if (cadenaAux[largoRegistroAux] == '-')
            offset *= -1;
        checkTruncado(offset,8);
        resultado=offset<<4|resultado; //puede ser que haya que hacer un corrimiento
        resultado<<20;
        resultado>>20;
    }
    return resultado;
}

int getOperando(int tipoOperando, char operandoEnString[]){
    char operandoAux[64];
    char* cono; 
    int resultado = -1;
    if (tipoOperando == TORegistro){
        resultado = operandoRegistro(operandoEnString);
    }
    else if (tipoOperando == TODirecto){
        if (operandoEnString[0] == '[')
            removeExtremos(operandoEnString,operandoEnString);
        if (operandoEnString[0] == '\'') //los que llevan comillas se asumen q son de 1 solo caracter
            resultado = (int)operandoEnString[1];
        else
            resultado = anyToInt(operandoEnString, &cono);
    }
    else if (tipoOperando == TOInmediato){
        if (operandoEnString[0] == '\'')
            resultado = (int)operandoEnString[1];
        else
            resultado = anyToInt(operandoEnString, &cono); 
    }
    else if (tipoOperando == TOIndirecto){
        resultado = operandoIndirecto(operandoEnString);
    }
    return resultado; // -1 = tipoOperando invalido
}

void getTablaSimbolos(char* nombreArchT){ //agregar por aca el exito 0 del duplicado
    FILE *archT;
    int auxint;
    char linea[largoLinea], **lineaParseada,aux[11],straux[largoLinea];
    if ((archT = fopen(nombreArchT, "r")) != NULL){
        while (fgets(linea, largoLinea, archT) != NULL){ 
            lineaParseada = parseline(linea);
            if (lineaParseada[0] != 0){ //si es rotulo
                if (checkSimbolo(lineaParseada[0])){
                    instruccion = 0xFFFFFFFF;
                    exito = 0;
                    printf("Simbolo duplicado: El rotulo %s ya se encuentra en la lista de simbolos", lineaParseada[0]);
                    printf("\n\n");
                }
                tablaSimbolos[cantSimbolos].tipo=0;
                tablaSimbolos[cantSimbolos].label = (char*) malloc(100);
                tablaSimbolos[cantSimbolos].contenido = (char*) malloc(100);
                upcaseString(lineaParseada[0],straux);
                strcpy(tablaSimbolos[cantSimbolos].label, straux);
                auxint=tempCS;
                itoa(auxint,straux,10);
                strcpy(tablaSimbolos[cantSimbolos].contenido,straux);
                cantSimbolos++;
                tempCS++;
            } 
            else if (lineaParseada[7] && lineaParseada[8]){ //si es constante
                if(checkSimbolo(lineaParseada[7])){
                    instruccion = 0xFFFFFFFF;
                    exito = 0;
                    printf("Simbolo duplicado: La constante %s ya se encuentra en la lista de simbolos", lineaParseada[7]);
                    printf("\n\n"); 
                }
                strcpy(aux,lineaParseada[8]);
                if (aux[0]=='"' && aux[strlen(aux)-1]=='"'){// define si string o numerico
                    tablaSimbolos[cantSimbolos].tipo=2;
                    removeExtremos(lineaParseada[8],lineaParseada[8]);
                }
                else
                    tablaSimbolos[cantSimbolos].tipo=1;
                tablaSimbolos[cantSimbolos].label = (char*) malloc(100);
                tablaSimbolos[cantSimbolos].contenido = (char*) malloc(100);
                upcaseString(lineaParseada[7],straux);
                strcpy(tablaSimbolos[cantSimbolos].label, straux);
                strcpy(tablaSimbolos[cantSimbolos].contenido,lineaParseada[8]);
                cantSimbolos++;
                cantString+=strlen(lineaParseada[8])+1;
            }
            else if (lineaParseada[1]) //si es mnemonico
                tempCS++;
        }
        fclose(archT);
    }
}

void upcaseString (char str[],char final[]){
char straux[largoString];
int i;    
    for (i=0;i<strlen(str);i++)
       straux[i]=toupper(str[i]);
    straux[i]='\0';
    strcpy(final,straux);
}

simbolo getSimbolos(char* operandoEnString){
    int i=0;
    char straux[largoLinea];
    upcaseString(operandoEnString,straux);
    while (i<cantSimbolos && strcmp(straux,tablaSimbolos[i].label)!=0)
        i++;    
    return tablaSimbolos[i]; //i es la pos de memoria correspondiente a ese Rotulo
}

int checkSalto(int mnemonico){
    if (mnemonico >= 0xf1 && mnemonico <= 0xf7)
        return 1;
    else
        return 0;
}

int checkSimbolo(char* cadena){ //check if simbolo esta en la tabla
int i=0;
        while (i<cantSimbolos && stricmp(cadena,tablaSimbolos[i].label))
            i++;
        if (i<cantSimbolos)
            return 1;
        else
            return 0;
}

int transformSimbolo(char *cadena){ //entra un formato de simbolo valido
simbolo auxRotulo;
char auxString[11];
int aux;
int aux2;
int resultado = 0;
    if (checkSimbolo(cadena)){
        auxRotulo=getSimbolos(cadena);
        if (auxRotulo.tipo==0 || auxRotulo.tipo==1) /* tipo=0 rotulo, tipo=1 cte num, tipo=2 cte string */
            strcpy(cadena,auxRotulo.contenido);
        else { /* tipo=2 cte string */
            itoa(auxRotulo.posCS,auxString,10);
            strcpy(cadena,auxString);
        }
        resultado = 1;
    }
    else {
        printf("Simbolo inexistente: Rotulo o constante %s no se encuentra en la lista de simbolos.", cadena); 
        printf("\n\n");
        instruccion = 0xFFFFFFFF;
        exito = 0;
    }
    return resultado;
}

void checkTruncado(int operando, int bits){
    if (bits == 16)
        if ((operando & 0xFFFF0000) != 0 && (operando & 0xFFFF0000)!=0XFFFF0000){ //considera el corriemiento para numeros negativos
            printf("Truncado de Operando: \n%d no puede ser almacenado en el espacio de %d bits. \nSe trunca la parte alta del valor.", operando, bits);
            printf("\n\n");
        }
    if (bits == 12)
        if ((operando & 0xFFFFF000) != 0  && (operando & 0xFFFFF000)!=0XFFFFF000){
            printf("Truncado de Operando: \n%d no puede ser almacenado en el espacio de %d bits. \nSe trunca la parte alta del valor.", operando, bits);
            printf("\n\n");
        }
    if (bits == 8)
        if ((operando & 0xFFFFFF00) != 0  && (operando & 0xFFFFFF00)!=0xFFFFFF00){
            printf("Truncado de Operando: \n%d no puede ser almacenado en el espacio de %d bits. \nSe trunca la parte alta del valor.", operando, bits);
            printf("\n\n");
        }
}

void printeo(int dirMem, int instruccion, char* lineaParseada[]){
    char coma[largoLinea]=";";
    if(lineaParseada[1]){ //tiene mnemonico --> linea comun
        if (lineaParseada[0] != 0) //si tiene rotulo
            printf("[%04d]:  %02X %02X %02X %02X %11s: %4s %11s %11s %s", dirMem, (instruccion>>24)&0xFF, (instruccion>>16)&0xFF, (instruccion>>8)&0xFF, (instruccion)&0xFF, 
            lineaParseada[0], lineaParseada[1], (lineaParseada[2] == 0) ? "" : lineaParseada[2],
            (lineaParseada[3] == 0) ? "" : lineaParseada[3], (lineaParseada[4] == 0) ? "" : strcat(coma,lineaParseada[4]));
        else
            printf("[%04d]:  %02X %02X %02X %02X %11d: %4s %11s %11s %s", dirMem, (instruccion>>24)&0xFF, (instruccion>>16)&0xFF, (instruccion>>8)&0xFF, (instruccion)&0xFF, 
            dirMem+1, lineaParseada[1], (lineaParseada[2] == 0) ? "" : lineaParseada[2], 
            (lineaParseada[3] == 0) ? "" : lineaParseada[3], (lineaParseada[4] == 0) ? "" : strcat(coma,lineaParseada[4]));
    }
    else if (lineaParseada[5]) //tiene segmento
        printf("\\\\ %s %s", lineaParseada[5], lineaParseada[6]);
    else if (lineaParseada[7]) //constante implicita
        printf("%s EQU %s", lineaParseada[7], lineaParseada[8]);
    else if (lineaParseada[4]) //comentario
        printf(lineaParseada[4]);
    printf("\n\n");
}

void getHeader(){
    header[0]=((int)'M'<<24 | (int)'V'<<16 | (int)'-'<<8 | (int)'2');
    header[1]=DS;
    header[2]=SS;
    header[3]=ES;
    header[4]=CS;
    header[5]=((int)'V'<<24 | (int)'.'<<16 | (int)'2'<<8 | (int)'2');
}

int updateSegmento(char* segmento, char* tamanio){
    int resultado = 1;
    char* cono;
    int valor = anyToInt(tamanio, &cono);
    if (stricmp(segmento, "DATA") == 0)
        DS = valor;
    else if (stricmp(segmento, "EXTRA") == 0)
        ES= valor;
    else if (stricmp(segmento, "STACK") == 0)
        SS = valor;
    else
        resultado = -1;
    return resultado;
}

void transformNull(char* cadena){
    char cadenaNula[] = "null";
    if (stricmp(cadena, cadenaNula) == 0){
        cadena[0] = '-';
        cadena[1] = '1';
        cadena[2] = '\0';
    }
}