#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#define TM 4096 //tamaño memoria
#define TR 16 //tamaño registro
#define DS 0  //accesos directos de los registros importantes
#define IP 5
#define CC 8
#define AC 9
#define EXTENDED_MASK 0xFFFFFFFF
#define REG_MASK 0xFFFF
#define HIGH_MASK 0xFF00
#define LOW_MASK 0x00FF

void LeeArch(int[],int[]);
void ExtraerDosOperandos(int ,int* ,int* );
typedef void T_fun(int *,int*);//tipo de funcion

int main(){
    int i,aux; //i: numero de operador aux: Almacena la instruccion a ejecutar
    int A,B;
    int memoria[TM],registro[TR];
    T_fun* ArrayFunc[]={}; //Arreglo de punteros a funciones, cuando las tengamos las metemos ahi  
    LeeArch(memoria,registro);
    while (registro[IP]<registro[DS]){
        i=0; 
        aux=memoria[registro[IP]];
        registro[IP]++;
        if ((aux&0xF0000000)==0xF0000000){
            i+=16;
            if((aux&0x0F000000)==0x0F000000){//0 OP
                i+=16;
                aux=(aux>>20)&0x0F;
            }
            else{//1 OP
                aux=(aux>>24)&0x00F;
            }
        }
        else{//2 OP
            ExtraerDosOperandos(aux,&A,&B);
            aux=(aux>>28)&0x0F;
        }
        i+=aux;
        //ArrayFunc[i-1];    EJECUTOR DE LAS FUNCIONES
        printf("%d\n",i);
    }     
    return 0;
}
void LeeArch(int memoria[],int registro[]){
    FILE* arch;
    arch=fopen("traducido.mv1","rb");
    int i=0;
    if (arch == NULL)
        printf("Error en la apertura. Es posible que el archivo no exista");
    fread(memoria+i,sizeof(int),1,arch);
    while(!feof(arch)){
        i++;
        fread(memoria+i,sizeof(int),1,arch);
    }
    registro[0]=i;
    fclose(arch);
}

void ExtraerDosOperandos(int aux,int* A,int* B){
    if (aux>>26 & 0x03==0){//Inmediato
        *A=aux>>12 & 0x0FFF;
    }
    else
        if (aux>>26 & 0x03==2){//Directo
            *A=get_value(A,EXTENDED_MASK);
        }
        else{//Registro
        }
    
}

int get_value(int *a, int mask){
    int result = *a & mask;
    if((mask&0xFF)==0){
        result=result>>8;
    }
    return result;
}
