#include <stdio.h>
#include <string.h>
#include <ctype.h>
#define TOInmediato (0)
#define TORegistro (1)
#define TODirecto (2)




int main(int argc, char const *argv[])
{
    const char* cadena = "ADD"; //por defecto se define como arary de char, luego se igual el puntero que apuunta a este array, en el puntero a char llamado cadena
    printf ("%d\n", mnemonico((char*)cadena));
    return 0;

}
// -------------------------------- mnemonico primera version --------------------------------
const char* op2Names[16] = {"MOV","ADD","SUB","SWAP","MUL","DIV","SHL","SUB","SHR","AND","OR","XOR"};//un array de punteros a char 
const char* op1Names[16] = {"SYS","JMP","JZ","JP","JN","JNZ","JNP","JNN","LDL","LDH","RND","NOT"};
const char* op0Names[16] = {"","STOP"};


int mnemonico(char* cadena){ //-1  no encontro el mnemonico
    for (int i=0; i<3; i++){
        if (stricmp(opNames[i], cadena) == 0){//i no importa mayusc
            return i;
        } 
    }
    return -1;
}
// -------------------------------- mnemonico segunda version --------------------------------
const char* tablaMnemonicos[3][16] = {{"","STOP","","","","","","","","","","","","","",""}, 
                            {"SYS","JMP","JZ","JP","JN","JNZ","JNP","JNN","LDL","LDH","RND","NOT","","","",""},
                            {"MOV","ADD","SUB","SWAP","MUL","DIV","SHL","SUB","SHR","AND","OR","XOR","","","",""}};


int mnemonico_(char* cadena){ //-1  no encontro el mnemonico
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

int checkInmediato(char* cadena){  
    if (cadena[0] == '%' || cadena[0] == '#' || cadena[0] == '@' || cadena[0] == '$')
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

int anyToIntH(char *s) //version ultraupgradeada
{
    const char* BASES = "**$*****@*#*****%";   
    char *bp = strchr(BASES, *s);
    char *Temp;
    if (bp != NULL){
        return strtol(s+1, &Temp, bp - BASES);
    }
    else 
    {
        return strtol(s, &Temp, 10);
    }
}

int operandoRegistro(char *operandoEnString){
    int largoCadena = strlen(operandoEnString);
    //pasar a mayusculas
    if (largoCadena == 2){ 
        if (toupper(operandoEnString[1]) == 'L')
            return (1<<4 | operandoEnString[0]);
        if (toupper(operandoEnString[1]) == 'H')
            return (2<<4 | operandoEnString[0]);
        if (toupper(operandoEnString[1]) == 'X')
            return (3<<4 | operandoEnString[0]);
    }
    if (largoCadena == 3){
        return (0 | operandoEnString[0]);
    }
}

int getOperando(int tipoOperando, char* operandoEnString){
    char operandoAux[64];
    char** cono;
    if (tipoOperando == TORegistro){
        return operandoRegistro(operandoEnString);
    }
    if (tipoOperando == TODirecto){
        removeCorchetes(operandoEnString, operandoAux);
        return anyToInt(operandoAux, cono);
    }
    if (tipoOperando == TOInmediato){
        return anyToInt(operandoEnString, cono);
    }
}