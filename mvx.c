#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#define TM 4096 //tamaño memoria
int ADecimal(char[]);
void LeeArch(int[]);
typedef void T_fun(int *,int*);//tipo de funcion


int main()
{
    int i,j=0,aux;//instruccion: Instruccion en decimal aux:Copia de instruccion para trabajar
    int memoria[TM];
    T_fun* ArrayFunc[]={}; //Arreglo de punteros a funciones, cuando las tengamos las metemos ahi  
    LeeArch(memoria);

    while (j<=6){//falta programar{
        i=0; 
        aux=memoria[j];
        if ((aux&0xF0000000)==0xF0000000){
            i+=16;
            if((aux&0x0F000000)==0x0F000000){
                i+=16;
                aux=(aux>>20)&0x0000000F;//0 OP
            }
            else{
                aux=(aux>>24)&0x0000000F;//1 OP
            }
        }
        else{
            aux=(aux>>28)&0x0000000F;//2 OP
        }
        i+=aux;
        j++;
        //ArrayFunc[i-1];    EJECUTOR DE LAS FUNCIONES
        printf("%d\n",i);
    }     
    return 0;
}


void LeeArch(int arreglo[]){
    FILE* arch;
    arch=fopen("traducido.mv1","rb");
    int i=0;
    if (arch == NULL)
        printf("Error en la apertura. Es posible que el archivo no exista");
    fread(arreglo+i,sizeof(int),1,arch);
    while(!feof(arch)){
        i++;
        fread(arreglo+i,sizeof(int),1,arch);
    }
    fclose(arch);
}