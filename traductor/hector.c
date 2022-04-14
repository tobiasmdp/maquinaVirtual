#include <stdio.h>
#include <string.h>
const char* opNames[16] = {"MOV","ADD","SUB"};//un array de punteros a char 

#define DUB(A)  (A+A)

int mnemonico(char* cadena){
    for (int i=0; i<3; i++){
        if (stricmp(opNames[i], cadena) == 0){
            return i;
        } //i no importa mayusc
    }
    return -1;
}

int main(int argc, char const *argv[])
{
    const char* cadena = "ADD"; //por defecto se define como arary de char, luego se igual el puntero que apuunta a este array, en el puntero a char llamado cadena
    printf ("%d\n", mnemonico((char*)cadena));
    return 0;

}

#define TOInmediato (0)
#define TORegistro (1)
#define TODirecto (2)

int checkNumeric(char* cadena){ //return 0 (false), return 1 (true)
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

int tipoOperando(char* cadena){
    if (checkDirecto(cadena))
        return TODirecto;
    if (checkInmediato(cadena))
        return TOInmediato;
    if (checkRegistro(cadena))
        return TORegistro;
    return -1;
}

void removeCorchetes(char* cadena, char* out){ //reemplazo el int con un void, eso se puede hacer? preguntar a Hector
    int largoCadena = strlen(cadena);
    memcpy(out,cadena+1, largoCadena-2);
    out [largoCadena-2] = 0;
}

int anyToInt(char *s, char **out){ //mira la de abajo a ver como se le pasa ese parametro
    const char* BASES = "**$*****@*#*****%";  ;
    int base = 10;
    char *bp = strchr(BASES, *s);
    if (bp != NULL){
        base = bp - BASES;
        ++s;
    }
    return strtol(s, out, base);
}

int anyToIntH(char *s)
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

int getOperando(int tipoOperando, char* operandoEnString){
    char* operandoAux;
    char** cono;
    if (tipoOperando == TODirecto){
        
    }
    if (tipoOperando == TORegistro){
        removeCorchetes(operandoEnString, operandoAux);
        return anyToInt(operandoAux, cono);
    }
    if (tipoOperando == TOInmediato){
        return anyToInt(operandoEnString, cono);
    }
}