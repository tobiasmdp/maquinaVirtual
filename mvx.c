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
void ExtraerUnOperando(int ,int**,int*, int[] ,int [],int *);
void ExtraerDosOperandos(int ,int** ,int** ,int*,int *, int[] ,int [],int *,int *);
int get_value(int *,int );
int set_value(int *,int , int);
void add(int *,int ,int *,int ,int ,int ,int [],int []);
typedef void T_fun(int *,int*);//tipo de funcion

int main(){
    int indicef,inst; //indicef: numero de operador aux: Almacena la instruccion a ejecutar
    int *A,*B,C,D,mascaraA,mascaraB;
    int memoria[TM],registro[TR];
    registro[10]=2;
    registro[11]=3;
    T_fun* ArrayFunc[]={}; //Arreglo de punteros a funciones, cuando las tengamos las metemos ahi  
    LeeArch(memoria,registro);
    while (registro[IP]<registro[DS]){
        indicef=0; 
        inst=memoria[registro[IP]];
        registro[IP]++;
        if ((inst&0xF0000000)==0xF0000000){
            indicef+=16;
            if((inst&0x0F000000)==0x0F000000){//0 OP (ya esta)
                indicef+=16;
                indicef+=(inst>>20)&0x0F;
            }
            else{//1 OP
                indicef+=(inst>>24)&0x00F;
                ExtraerUnOperando(inst,&A,&C,memoria,registro,&mascaraA);
            }
        }
        else{//2 OP
            ExtraerDosOperandos(inst,&A,&B,&C,&D,memoria,registro,&mascaraA,&mascaraB);
            indicef+=(inst>>28)&0x0F;
        }
        add(A,mascaraA,B,C,D,mascaraB,memoria,registro);
        //ArrayFunc[i-1];    EJECUTOR DE LAS FUNCIONES
        printf("%d\n",(inst>>12 & 0x0FFF));
    }     
    return 0;
}
void LeeArch(int memoria[],int registro[]){
    FILE* arch;
    arch=fopen("traducido2.mv1","rb");
    int i=0;
    if (arch == NULL)
        printf("Error en la apertura. Es posible que el archivo no exista");
    fread(memoria+i,sizeof(int),1,arch);
    while(!feof(arch)){
        i++;
        fread(memoria+i,sizeof(int),1,arch);
    }
    registro[DS]=i;
    fclose(arch);
}

void ExtraerUnOperando(int inst,int **A,int *C, int memoria[],int registro[],int *mascaraA){
    if ((inst>>22 & 0x03)==0){//Inmediato
        *C=inst & 0x0FFFF;
        *A=C; // tambien analizar si es char
   }
    else
        if (inst>>22 & 0x03==2)//Directo
            *A=&memoria[registro[DS]+(inst & 0x0FFFF)];
        else{//Registro
           *A=&registro[(inst & 0x0F)];
           if (inst>>4 & 0x03==1) // registro del 4to byte
             *mascaraA=LOW_MASK;
           else
                 if (inst>>4 & 0x03==2) // registro del 3 byte
                   *mascaraA=HIGH_MASK;
                else //registro de 2 bytes
                  if (inst>>4 & 0x03==3)
                    *mascaraA= REG_MASK;
                 //registro de 4 bytes (sale automatico)
        }
}

void ExtraerDosOperandos(int inst,int **A,int **B,int *C, int *D, int memoria[],int registro[],int *mascaraA, int *mascaraB){
    //primer operando
    *mascaraA=*mascaraB=EXTENDED_MASK;
    if ((inst>>26 & 0x03)==0){//Inmediato
        *C=inst>>12 & 0x0FFF;
        *A=C; // tambien analizar si es char
   }
    else
        if (inst>>26 & 0x03==2)//Directo
            *A=&memoria[registro[DS]+(inst>>12 & 0x0FFF)];
        else{//Registro
           *A=&registro[(inst>>12 & 0x0F)];
           if (inst>>16 & 0x03==1) // registro del 4to byte
             *mascaraA=LOW_MASK;
           else
                 if (inst>>16 & 0x03==2) // registro del 3 byte
                   *mascaraA=HIGH_MASK;
                else //registro de 2 bytes
                  if (inst>>16 & 0x03==3)
                    *mascaraA= REG_MASK;
                 //registro de 4 bytes (sale automatico)
        }
    //segundo operando
    if ((inst>>24 & 0x03)==0){//Inmediato
        *D=inst & 0x0FFF;
        *B=D; // tambien analizar si es char
   }
    else
        if (inst>>24 & 0x03==2)//Directo
            *B=&memoria[registro[DS]+(inst & 0x0FFF)];
        else{//Registro
           *B=&registro[(inst & 0x0F)];
           if (inst>>4 & 0x03==1) // registro del 4to byte
             *mascaraB=LOW_MASK;
           else
                 if (inst>>4 & 0x03==2) // registro del 3 byte
                   *mascaraB=HIGH_MASK;
                else //registro de 2 bytes
                  if (inst>>4 & 0x03==3)
                    *mascaraB= REG_MASK;
                 //registro de 4 bytes (sale automatico)
        }
}

void add(int *A,int mascaraA,int *B,int C,int D,int mascaraB,int memoria[],int registro[]){
    int auxA=get_value(A,mascaraA);
    int auxB=get_value(B,mascaraB);
    set_value (A,auxA+auxB,mascaraA);
}

int get_value(int *a, int mask){
    int result = *a & mask;
    if((mask&0xFF)==0){
        result=result>>8;
    }
    return result;
}

int set_value (int *a,int value, int mask){
    if ((mask & 0xFF)==0){
        value = value << 8;
    }
    *a=*a & ~mask;
    *a = *a | (value & mask);
}