#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#define TR 33
int ADecimal(char[]);
int main()
{
    int i,instruccion,aux;//instruccion: Instruccion en decimal aux:Copia de instruccion para trabajar
    char registro[TR];// Char de 32 bytes para sacar de archivo

    FILE* arch;
    arch=fopen("traducido.txt","r");
    if (arch == NULL)
        printf("Error en la apertura. Es posible que el archivo no exista");
    fscanf(arch,"%s",registro);
    while(!feof(arch)){ 
        instruccion=ADecimal(registro);
        //aux=instruccion>>28;
        /*if (aux&0xF0000000==0xF0000000){
            i+=16;
            if(aux&0x0F000000==0x0F000000){
                i+=16;
            }
        }
        else{
            i+=ADecimal()
        }*/
        printf("%d\n",instruccion);
        fscanf(arch,"%s",registro);
    }
    return 0;
}


int ADecimal(char arreglo []){//Transforma char en binario a decimal de 32 bits con signo
    int aux=0,i,N;
    N=strlen(arreglo)-1;
    for (i=0;i<N;i++){
        if(arreglo[N-i]=='1'){
            aux+=pow(2,i);
            printf("%d   %d  \n",i,aux);
        }
    }
    if(arreglo[0]=='1')
            aux*=-1;
    return aux;
}

