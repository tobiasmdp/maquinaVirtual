#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <conio.h>

#define TM 8192 //tamaño memoria
#define TR 16 //tamaño registro
#define DS 0  //accesos directos de los registros importantes
#define SS 1
#define ES 2
#define CS 3
#define HP 4
#define IP 5
#define SP 6
#define BP 7
#define CC 8
#define AC 9
#define EAX 10
#define EBX 11
#define ECX 12
#define EDX 13
#define EEX 14
#define EFX 15
#define EXTENDED_MASK 0xFFFFFFFF
#define REG_MASK 0xFFFF
#define HIGH_MASK 0xFF00
#define LOW_MASK 0x00FF

/*----------------------------------------------Prototipos----------------------------------------------------------------*/

void LeeArch(int[],int[]);
void ExtraerUnOperando(int ,int**,int*, int[] ,int [],int *);
void ExtraerDosOperandos(int ,int** ,int** ,int*,int *, int[] ,int [],int *,int *);
void Extractor(int , int ,int **,int *, int [],int [],int *);
void MOV(int *,int ,int *,int ,int ,int ,int [],int []);
void ADD(int *,int ,int *,int ,int ,int ,int [],int []);
void SUB(int *,int ,int *,int ,int ,int ,int [],int []);
void MUL(int *,int ,int *,int ,int ,int ,int [],int []);
void DIV(int *,int ,int *,int ,int ,int ,int [],int []);
void SLEN(int *,int ,int *,int ,int ,int ,int [],int []);
void SMOV(int *,int ,int *,int ,int ,int ,int [],int []);
void SCMP(int *,int ,int *,int ,int ,int ,int [],int []);
void SWAP(int *,int ,int *,int ,int ,int ,int [],int []);
void CMP(int *,int ,int *,int ,int ,int ,int [],int []);
void SHL(int *,int ,int *,int ,int ,int ,int [],int []);
void SHR(int *,int ,int *,int ,int ,int ,int [],int []);
void SYS(int *,int ,int *,int ,int ,int ,int [],int []);
void JMP(int *,int ,int *,int ,int ,int ,int [],int []);
void JZ(int *,int ,int *,int ,int ,int ,int [],int []);
void JP(int *,int ,int *,int ,int ,int ,int [],int []);
void JN(int *,int ,int *,int ,int ,int ,int [],int []);
void JNZ(int *,int ,int *,int ,int ,int ,int [],int []);
void JNP(int *,int ,int *,int ,int ,int ,int [],int []);
void JNN(int *,int ,int *,int ,int ,int ,int [],int []);
void AND(int *,int ,int *,int ,int ,int ,int [],int []);
void OR(int *,int ,int *,int ,int ,int ,int [],int []);
void XOR(int *,int ,int *,int ,int ,int ,int [],int []);
void LDL(int *,int ,int *,int ,int ,int ,int [],int []);
void LDH(int *,int ,int *,int ,int ,int ,int [],int []);
void RND(int *,int ,int *,int ,int ,int ,int [],int []);
void NOT(int *,int ,int *,int ,int ,int ,int [],int []);
void PUSH(int *,int ,int *,int ,int ,int ,int [],int []);
void POP(int *,int ,int *,int ,int ,int ,int [],int []);
void CALL(int *,int ,int *,int ,int ,int ,int [],int []);
void RET(int *,int ,int *,int ,int ,int ,int [],int []);
void STOP(int *,int ,int *,int ,int ,int ,int [],int []);

int get_value(int *,int );
void set_value(int *,int , int);
void registroCC(int, int[]);
int checkFlag(char []);
void breakpoint(int [], int []);
void dividenum(char [],int *, int *);
void disassembler(int [],int []);
void imprimeOperando(int , int);
int low(int);
int high(int);
int dirmemoria(int , int [], int[]);
int dirinversa(int *, int [],int []);
typedef void T_fun(int *,int ,int *,int ,int ,int ,int [],int []);//tipo de funcion

/*----------------------------------------------------Comienzo del programa-----------------------------------------------*/

int p=0; /*si aparece p en el sys se cambia*/
int pri=0;
int main(int argc, char const *argv[]){ // VER BIEN LOS ARGUMENTOS
    int indicef,inst; //indicef: numero de operador aux: Almacena la instruccion a ejecutar
    int *A,*B,C,D,mascaraA,mascaraB;
    int memoria[TM],registro[TR]={0};
    T_fun* ArrayFunc[]={MOV,ADD,SUB,SWAP,MUL,DIV,CMP,SHL,SHR,AND,OR,XOR,SLEN,SMOV,SCMP,0,SYS,JMP,JZ,JP,JN,JNZ,JNP,JNN,LDL,LDH,RND,NOT,PUSH,POP,CALL,0,RET,STOP}; //Arreglo de punteros a funciones, cuando las tengamos las metemos ahi  
    
    if (checkFlag("-c"))  /*limpio la pantalla al inicio si -c esta como argumento*/
        system("cls");
    
    LeeArch(memoria,registro);
    if (checkFlag("-d"))
        disassembler(registro,memoria);
    while (dirmemoria(registro[IP],registro,memoria)<=(high(registro[CS]) + low(registro[CS])) && dirmemoria(registro[IP],registro,memoria)>=(low(registro[CS]))){
        indicef=0; 
        inst=memoria[dirmemoria(registro[IP],registro,memoria)];
        registro[IP]++;
        if ((inst&0xF0000000)==0xF0000000){
            indicef+=16;
            if((inst&0x0F000000)==0x0F000000){//0 OP
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
        (*ArrayFunc[indicef])(A,mascaraA,B,C,D,mascaraB,memoria,registro);
        if (checkFlag("-b") && p==1 && pri>1){
            if (checkFlag("-c"))  /*limpio la pantalla al inicio si -c esta como argumento*/
                system("cls");
            if (checkFlag("-d"))
                disassembler(registro,memoria);
            breakpoint(registro,memoria);
        }
        if(pri==1)
             pri++;
    }     
    return 0;
}

void LeeArch(int memoria[],int registro[]){
    FILE* arch;
    arch=fopen(_argv[1],"rb");
    int header[6],i=0,aux;
    if (arch == NULL)
        printf("Error en la apertura. Es posible que el archivo no exista");
    fread(header+i,sizeof(int),1,arch);
    while(!feof(arch) && i<5){
        i++;
        fread(header+i,sizeof(int),1,arch);
    }
    
   if((header[0]>>24&0x0FF)!='M' || (header[0]>>16&0x0FF)!='V' || (header[0]>>8&0x0FF)!='-' || (header[0]&0x0FF)!='1' || (header[5]>>24&0x0FF)!='V' || (header[5]>>16&0x0FF)!='.' || (header[5]>>8&0x0FF)!='2' || (header[5]&0x0FF)!='2') {
        printf("El formato del archivo .mv2 no pudo ser validado");
        exit(EXIT_FAILURE);
    } 
    else{
        printf("El archivo fue validado\n");
    }
    if(header[1]+header[2]+header[3]+header[4]>TM){
        printf("Los valores asignados a los segmentos exceden el tamaño de la memoria.");
        exit(EXIT_FAILURE);
    }
    i=0;
    registro[CS]=header[4];
    registro[CS]<<=16;
    registro[CS]|=(REG_MASK & i);
    i+=header[4];

    registro[DS]=header[1];
    registro[DS]<<=16;
    registro[DS]|=(REG_MASK & i);
    i+=header[1];

    registro[ES]=header[3];
    registro[ES]<<=16;
    registro[ES]|=(REG_MASK & i);
    i+=header[3];

    registro[SS]=header[2];
    registro[SS]<<=16;
    registro[SS]|=(REG_MASK & i);

    registro[HP]=2;
    registro[HP]<<=16;
    registro[HP]|=(REG_MASK & 0);

    registro[IP]=3;
    registro[IP]<<=16;
    registro[IP]|=(REG_MASK & 0);

    aux=registro[SS]>>16;
    registro[SP]=1;
    registro[SP]<<=16;
    registro[SP]|=(REG_MASK & aux);

    registro[BP]=1;
    registro[BP]<<=16;
    i=0;
    fread(memoria+i,sizeof(int),1,arch);
    while(!feof(arch)){
        i++;
        fread(memoria+i,sizeof(int),1,arch);
    }
    fclose(arch);
}

/*-----------------------------------------------------Extraccion operandos------------------------------------------------------*/

void ExtraerUnOperando(int inst,int **A,int *C, int memoria[],int registro[],int *mascaraA){
    *mascaraA=EXTENDED_MASK;
    int TipoOp=inst>>22 & 0x03;
    int Operando=inst&0xFFFF;
    Extractor(TipoOp,Operando,A,C,memoria,registro,mascaraA);
}

void ExtraerDosOperandos(int inst,int **A,int **B,int *C, int *D, int memoria[],int registro[],int *mascaraA, int *mascaraB){
    *mascaraA=*mascaraB=EXTENDED_MASK;
    int TipoOp,Operando;

    TipoOp=inst>>26 & 0x03;
    Operando=(inst>>12)&0xFFF;
    Extractor(TipoOp,Operando,A,C,memoria,registro,mascaraA);

    TipoOp=inst>>24 & 0x03;
    Operando=inst&0xFFF;
    Extractor(TipoOp,Operando,B,D,memoria,registro,mascaraB);

}

void Extractor(int TipoOp, int operando ,int **A,int *C, int memoria[],int registro[],int *mascaraA){
    int TipoReg=operando>>4 & 0x03;
    int numreg,offset,direccion,inicioReg,finReg;
    if (TipoOp==0){                                                         // Inmediato
            *C=operando;
            *C<<=16; 
            *C>>=16;
            *A=C;
        }
        else
            if (TipoOp==2)                                                  // Directo
                *A=&memoria[low(registro[DS])+operando];
            else
                if(TipoOp=1){                                               // Registro
                    *A=&registro[operando & 0x0F];
                    if (TipoReg==1)                                               // Registro del 4TO byte
                        *mascaraA=LOW_MASK;
                    else if (TipoReg==2)                                          // Registro del 3RO byte
                        *mascaraA=HIGH_MASK;
                    else if (TipoReg==3)                                          // Registro de 3RO Y 4TO bytes
                        *mascaraA= REG_MASK;           
                }                                                                 // Registro de los 4 bytes sale directo
                else{                                                       // Indirecto
                    numreg=registro[operando & 0x0F];
                    offset=(operando>>4) & 0x0FF;
                    offset<<=24;
                    offset>>=24;
                    inicioReg=low(registro[high(registro[numreg])]);// Direccion absoluta de donde arranca el registro asociado en la parte alta del registro del operando
                    finReg=memoria[dirmemoria(registro[numreg],registro,memoria)];// Direccion absoluta de donde termina el registro asociado en la parte alta del registro del operando
                    direccion=inicioReg+low(registro[numreg])+offset;
                    if(direccion>inicioReg && direccion<finReg)
                        *A=&memoria[direccion];
                    else{
                        printf("Segmentation fault");
                        exit(EXIT_FAILURE);
                    }
                }
}


/*--------------------------------------------------Funciones de 2 operandos:-------------------------------------------------*/

void MOV(int *A,int mascaraA,int *B,int C,int D,int mascaraB,int memoria[],int registro[]){
    int auxB=get_value(B,mascaraB);
    set_value (A,auxB,mascaraA);
}

void ADD(int *A,int mascaraA,int *B,int C,int D,int mascaraB,int memoria[],int registro[]){
    int auxA=get_value(A,mascaraA);
    int auxB=get_value(B,mascaraB);
    registroCC(auxA+auxB,registro);
    set_value (A,auxA+auxB,mascaraA);
}

void SUB(int *A,int mascaraA,int *B,int C,int D,int mascaraB,int memoria[],int registro[]){
    int auxA=get_value(A,mascaraA);
    int auxB=get_value(B,mascaraB);
    registroCC(auxA-auxB,registro);
    set_value (A,auxA-auxB,mascaraA);
}


void SWAP(int *A,int mascaraA,int *B,int C,int D,int mascaraB,int memoria[],int registro[]){
    int aux=get_value(A,mascaraA);
    set_value(A,get_value(B,mascaraB),mascaraA);
    set_value(B,aux,mascaraB);
}

void MUL(int *A,int mascaraA,int *B,int C,int D,int mascaraB,int memoria[],int registro[]){
    int auxA=get_value(A,mascaraA);
    int auxB=get_value(B,mascaraB);
    registroCC(auxA*auxB,registro);
    set_value (A,auxA*auxB,mascaraA);
}

void DIV(int *A,int mascaraA,int *B,int C,int D,int mascaraB,int memoria[],int registro[]){
    int auxA=get_value(A,mascaraA);
    int auxB=get_value(B,mascaraB);
    registroCC(auxA/auxB,registro);
    set_value (A,auxA/auxB,mascaraA);
    set_value (registro+AC,auxA%auxB,mascaraA);
}

void CMP(int *A,int mascaraA,int *B,int C,int D,int mascaraB,int memoria[],int registro[]){
    int auxA=get_value(A,mascaraA);
    int auxB=get_value(B,mascaraB);
    registroCC(auxA-auxB,registro);
}

void AND(int *A,int mascaraA,int *B,int C,int D,int mascaraB,int memoria[],int registro[]){
    int auxA=get_value(A,mascaraA);
    int auxB=get_value(B,mascaraB);
    registroCC(auxA&auxB,registro);
    set_value (A,auxA&auxB,mascaraA);
}

void OR(int *A,int mascaraA,int *B,int C,int D,int mascaraB,int memoria[],int registro[]){
    int auxA=get_value(A,mascaraA);
    int auxB=get_value(B,mascaraB);
    registroCC(auxA|auxB,registro);
    set_value (A,auxA|auxB,mascaraA);
}

void XOR(int *A,int mascaraA,int *B,int C,int D,int mascaraB,int memoria[],int registro[]){
    int auxA=get_value(A,mascaraA);
    int auxB=get_value(B,mascaraB);
    registroCC(auxA^auxB,registro);
    set_value (A,auxA^auxB,mascaraA);
}

void SLEN(int *A,int mascaraA,int *B,int C,int D,int mascaraB,int memoria[],int registro[]){
    int auxB=get_value(B,mascaraB);
    int i=0;
    while (auxB!=0){
        i++;
        B++;
        auxB=get_value(B,mascaraB);
    }
    set_value(A,i,mascaraA);
}

void SMOV(int *A,int mascaraA,int *B,int C,int D,int mascaraB,int memoria[],int registro[]){
    int auxB=get_value(B,mascaraB);
    while (auxB!=0){
        set_value(A,auxB,mascaraA);
        A++;
        B++;
        auxB=get_value(B,mascaraB);
    }
    set_value(A,0,mascaraA);
}


void SCMP(int *A,int mascaraA,int *B,int C,int D,int mascaraB,int memoria[],int registro[]){
    int auxA=get_value(A,mascaraA);
    int auxB=get_value(B,mascaraB);
    registro[CC]=0;
    while ((auxA!=0 || auxB!=0) && (registro[CC]&1)==1){
        registroCC(auxA-auxB,registro);
        A++;
        B++;
        auxB=get_value(B,mascaraB);
        auxA=get_value(A,mascaraA);
    }
}

void SHL(int *A,int mascaraA,int *B,int C,int D,int mascaraB,int memoria[],int registro[]){
    int auxA=get_value(A,mascaraA);
    int auxB=get_value(B,mascaraB);
    registroCC(auxA<<auxB,registro);
    set_value (A,auxA<<auxB,mascaraA);
}

void SHR(int *A,int mascaraA,int *B,int C,int D,int mascaraB,int memoria[],int registro[]){
    int auxA=get_value(A,mascaraA);
    int auxB=get_value(B,mascaraB);
    registroCC(auxA>>auxB,registro);
    set_value (A,auxA>>auxB,mascaraA);
}


/*-----------------------------------------------------Funciones de 1 operando:------------------------------------------------*/ 

void SYS(int *A,int mascaraA,int *B,int C,int D,int mascaraB,int memoria[],int registro[]){ //acomodar las variables
    int indice,contc,cont=0,longitud,i;
    char caracter[255];
    indice=dirmemoria(registro[EDX],registro,memoria);

    if ((*A & mascaraA) == 1){// lectura // revisar contadores
        while (cont<(registro[ECX]&REG_MASK)){
            if (( registro[EAX]&0x0800)>>11==0)
                printf("[%04d]: ",(indice+cont));
            if ((registro[EAX]&0x0100)>>8==0){
                if ((registro[EAX]&0x001)==1)
                    scanf("%d",memoria+(indice+cont));
                if ((registro[EAX]&0x004)==1)
                    scanf("%o",memoria+(indice+cont));
                if ((registro[EAX]&0x008)==1)
                    scanf("%X",memoria+(indice+cont));
                cont++;
            }
            else {
                scanf("%s",caracter);
                longitud=strlen(caracter);
                contc=0;
                while (contc<longitud && cont<(registro[ECX]&REG_MASK)){
                    memoria[indice+cont]=caracter[contc];
                    contc++;
                    cont++;
                }
            }
        }
    }
    else 
      if ((*A & mascaraA) == 2){ // escritura
        cont=0;
        while (cont<(registro[ECX]&REG_MASK)){
            if ((registro[EAX]&0x0800)>>11==0)
                printf("[%04d]: ",(indice+cont));
            if ((registro[EAX]&0x001)==1)
                printf("%4d ",memoria[indice+cont]);
            if ((registro[EAX]&0x004)==1)
                printf("%4o ",memoria[indice+cont]);
            if ((registro[EAX]&0x008)==1)
                printf("%08X ",memoria[indice+cont]);
            if ((registro[EAX]&0x010)>>4==1)
                if ((memoria[indice+cont]&0x0FF<=31) || (memoria[indice+cont]&0x0FF==127))
                    printf(".");
                else
                    printf("%c ",(memoria[indice+cont]&0x0FF));
            if ((registro[EAX]&0x0100)>>8==0)
                printf("\n");
            cont++;
        }
      }
      else
        if ((*A & mascaraA) == 15){ //breakpoint - flags
            if (checkFlag("-c"))
                system("cls");
            if (checkFlag("-d")){
                disassembler(registro,memoria);
            if (checkFlag("-b"))
                breakpoint(registro,memoria);
            }
        }
        else
            if ((*A & mascaraA) == 3){ //String read
                if ((registro[EAX]&0x0800)>>11==0)
                    printf("[%04d]: ",(indice+cont));
                scanf("%s",caracter);
                i=0;
                while (i<strlen(caracter) && i<registro[ECX]&REG_MASK){
                    memoria[indice+i]=caracter[i];
                    i++;
                }
                memoria[indice+registro[ECX]&REG_MASK]='\0';
            }
            else if ((*A & mascaraA) == 4){ //String write
                if ((registro[EAX]&0x0800)>>11==0)
                    printf("[%04d]: ",(indice+cont));
                for (int i = 0; i < registro[ECX]&REG_MASK; i++){
                    printf("%c ",(memoria[indice+cont]&0x0FF));
                }
                if ((registro[EAX]&0x0100)>>8==0)
                    printf("\n");
            }
            else
                if ((*A & mascaraA) == 7) //clear screen
                    system("cls");    
}

void JMP(int *A,int mascaraA,int *B,int C,int D,int mascaraB,int memoria[],int registro[]){
    registro[IP]&=0xFFFF0000;
    registro[IP]|=get_value(A,mascaraA)& REG_MASK;
    
    
    //Como en la especificacion de la parte 1 
}

void JZ(int *A,int mascaraA,int *B,int C,int D,int mascaraB,int memoria[],int registro[]){
    if ((registro[CC]&0x01)==1)
        JMP(A,mascaraA,B,C,D,mascaraB,memoria,registro);
}

void JP(int *A,int mascaraA,int *B,int C,int D,int mascaraB,int memoria[],int registro[]){ 
    if ((registro[CC]&0x80000000)==0 && (registro[CC]&0x01)==0)
        JMP(A,mascaraA,B,C,D,mascaraB,memoria,registro);
}

void JN(int *A,int mascaraA,int *B,int C,int D,int mascaraB,int memoria[],int registro[]){
    if ((registro[CC]>>31)&0x01==1 && (registro[CC]&0x01)==0)
        JMP(A,mascaraA,B,C,D,mascaraB,memoria,registro);
}

void JNZ(int *A,int mascaraA,int *B,int C,int D,int mascaraB,int memoria[],int registro[]){
    if ((registro[CC]&0x01)==0)
        JMP(A,mascaraA,B,C,D,mascaraB,memoria,registro);
}

void JNP(int *A,int mascaraA,int *B,int C,int D,int mascaraB,int memoria[],int registro[]){
    if (((registro[CC]&0x01)==1 && (registro[CC]&0x80000000)==0) || ((registro[CC]&0x80000000)>>31==1 && (registro[CC]&0x1)==0))
        JMP(A,mascaraA,B,C,D,mascaraB,memoria,registro);
}

void JNN(int *A,int mascaraA,int *B,int C,int D,int mascaraB,int memoria[],int registro[]){
    if (((registro[CC]&0x01)==1 && ((registro[CC]&0x80000000))==0) || ((registro[CC]&0x80000000)>>31==0 && (registro[CC]&0x01)==0))
        JMP(A,mascaraA,B,C,D,mascaraB,memoria,registro);
}

void LDH(int *A,int mascaraA,int *B,int C,int D,int mascaraB,int memoria[],int registro[]){
    int auxA=get_value(A,mascaraA);
    auxA<<=8;
    set_value (&registro[AC],auxA,0xFFFF0000);
}

void LDL(int *A,int mascaraA,int *B,int C,int D,int mascaraB,int memoria[],int registro[]){
    int auxA=get_value(A,mascaraA);
    set_value (&registro[AC],auxA,0x0FFFF);
}

void RND(int *A,int mascaraA,int *B,int C,int D,int mascaraB,int memoria[],int registro[]){
    int auxA=get_value(A,mascaraA);
    set_value (&registro[AC],rand()%auxA,EXTENDED_MASK);
}

void NOT(int *A,int mascaraA,int *B,int C,int D,int mascaraB,int memoria[],int registro[]){
    int auxA=get_value(A,mascaraA);
    registroCC(~auxA,registro);
    set_value (A,~auxA,mascaraA);
}
void PUSH(int *A,int mascaraA,int *B,int C,int D,int mascaraB,int memoria[],int registro[]){
    int auxA=get_value(A,mascaraA);
    if(low(registro[SS])==low(registro[SP])){
        printf("Stack overflow. El programa finalizo incorrectamente");
        exit(EXIT_FAILURE);
    }
    registro[SP]--;
    set_value (memoria+dirmemoria(registro[SP],registro,memoria),auxA,mascaraA);
}
void POP(int *A,int mascaraA,int *B,int C,int D,int mascaraB,int memoria[],int registro[]){
    if(low(registro[SS])+high(registro[SS])==low(registro[SP])){
        printf("Stack underoverflow. El programa finalizo incorrectamente");
        exit(EXIT_FAILURE);
    }
    int auxA=get_value(memoria+dirmemoria(registro[SP],registro,memoria),EXTENDED_MASK);
    registro[SP]++;
    set_value (A,auxA,EXTENDED_MASK);
}
void CALL(int *A,int mascaraA,int *B,int C,int D,int mascaraB,int memoria[],int registro[]){
    int auxA=get_value(A,mascaraA);
    PUSH(memoria+dirmemoria(registro[IP],registro,memoria),mascaraA,B,C,D,mascaraB,memoria,registro);//Quiza sea mejor que dir memoria me devuelva un puntero? Curioso 
    JMP(A,mascaraA,B,C,D,mascaraB,memoria,registro);
}

/*Funciones de 0 operandos:*/
void RET(int *A,int mascaraA,int *B,int C,int D,int mascaraB,int memoria[],int registro[]){
    if(low(registro[SS])+high(registro[SS])==low(registro[SP])){
        printf("Stack underflow. El programa finalizo incorrectamente");
        exit(EXIT_FAILURE);
    }
    JMP(memoria+dirmemoria(registro[SP],registro,memoria),EXTENDED_MASK,B,C,D,mascaraB,memoria,registro);
    registro[SP]++;
}

void STOP(int *A,int mascaraA,int *B,int C,int D,int mascaraB,int memoria[],int registro[]){
    registro[IP]=(high(registro[CS]) + low(registro[CS])+1);
}

/*Otras funciones utiles*/

void registroCC(int aux, int registro[]){
    registro[CC]=0x00000000;
    if(aux<0)
        registro[CC]|=0x80000000; 
    if (aux==0)
        registro[CC]|=0x01;
}

int get_value(int *a, int mask){
    int result = *a & mask;
    if((mask&0xFF)==0){
        result=result>>8;
    }
    
    return result;
}

void set_value (int *a,int value, int mask){
    if ((mask & 0xFF)==0){
        value = value << 8;
    }
    *a=*a & ~mask;
    *a = *a | (value & mask);
}

int low(int a){//accesos rapidos a la parte baja de un registro para no perder legibilidad, solo debe utilizarse exclusivamente para registros de procesador
    return a & REG_MASK;
}
int high(int a){
    return (a>>16) & REG_MASK;
}

int dirmemoria(int inst, int registro[],int memoria[]){
    int resultado,codseg=high(inst);

    if(codseg==0){
        resultado=low(inst)+ low(registro[DS]);
    }
    else if(codseg==1){
        resultado=low(inst)+low(registro[SS]);
    }
    else if (codseg==2){
        resultado=low(inst)+low(registro[ES]);
    }
    else if(codseg==3){
        resultado=low(inst)+low(registro[CS]);
    }
    return resultado;//Direccion absoluta del arreglo memoria
}
/*
int dirinversa(int *dir, int registro[],int memoria[]){
    int aux=(int)(dir-memoria);
    int resultado;
    if(aux>=low(registro[DS]) && aux<= (high(registro[DS]) + low(registro[DS]))){//Si esta dentro del DS
        resultado=DS;
        resultado<<=16;
        resultado|= aux-low(registro[DS]);
    }
    else if(aux>=low(registro[SS]) && aux<= (high(registro[SS]) + low(registro[SS]))){
        resultado=SS;
        resultado<<=16;
        resultado|= aux-low(registro[SS]);
    }
    else if(aux>=low(registro[ES]) && aux<= (high(registro[ES]) + low(registro[ES]))){
        resultado=ES;
        resultado<<=16;
        resultado|= aux-low(registro[ES]);
    }
    else if(aux>=low(registro[CS]) && aux<= (high(registro[CS]) + low(registro[CS]))){//Si esta dentro del CS (ESTE ES EL QUE SE DEBERIA EJECUTAR SIEMPRE)
        resultado=CS;
        resultado<<=16;
        resultado|= aux-low(registro[CS]);
    
    return resultado;
}
*/
int checkFlag(char bandera []){ 
    int i=0;
    while (i<_argc && strcmp(_argv[i],bandera)!=0)
        i++;
    return (i==_argc)?0:1;
}

void breakpoint(int registro[], int memoria[]){
    char sig[10];
    int aux,aux1;
    
    printf("[%04d] cmd: ",dirmemoria(registro[IP],registro,memoria)-1);
    fgets(sig,10,stdin);
    while (sig[0]!='r' && sig[0]!='p'){
        dividenum(sig,&aux,&aux1);
        if (aux1==-1)
          printf("[%04d] %X %d \n",aux,memoria[aux],memoria[aux]);
        else
          for (int i=aux;i<=aux1;i++)
            printf("[%04d] %08X %4d \n",i,memoria[i],memoria[i]);
        printf("[%04d] cmd: ",dirmemoria(registro[IP],registro,memoria)-1);
        fgets(sig,10,stdin);
    }
    if (sig[0]=='p'){
      p=1;
      pri++;
    }
    else
      p=0;
}

void dividenum(char sig[],int *aux, int *aux1){
    char cadaux[10],cadaux1[10]="-1";
    int i=0,j=0,maxcad=strlen(sig);
    while (i<=maxcad && sig[i]!=' '){
     cadaux[i]=sig[i];
     i++;
     }
     cadaux[i]='\0';
     if (i<=maxcad && sig[i]==' '){
         i++;
         while (i<=maxcad){
          cadaux1[j]=sig[i];
          j++;
          i++;
        }
     cadaux1[j]='\0';
     }
    *aux=atoi(cadaux);
    *aux1=atoi(cadaux1);
}

void disassembler(int registro[],int memoria[]){ // ver impresion dessassembler
char* Mnemonicos[34]= {"MOV","ADD","SUB","SWAP","MUL","DIV","CMP","SHL","SHR","AND","OR","XOR","SLEN","SMOV","SCMP","0","SYS","JMP","JZ","JP","JN","JNZ","JNP","JNN","LDL","LDH","RND","NOT","PUSH","POP","CALL","0","RET","STOP"};
char* Procesador[16]={"DS","SS","ES","CS","HP","IP","SP","BP","CC","AC","EAX","EBX","ECX","EDX","EEX","EFX"};
int indiceM,indiceP,j,i=dirmemoria(registro[IP],registro,memoria),tipoOp;
    printf("Codigo: \n");
    if(i>5)
        i-=5;
    else
        i=0;
    while (i<low(registro[DS]) && i<5+low(registro[IP])){
        if (i==low(registro[IP]))
            printf(">");
        else
            printf(" ");
        indiceM=0;
        if ((memoria[i]&0xF0000000)==0xF0000000){
            indiceM+=16;
            if((memoria[i]&0x0F000000)==0x0F000000){                    //0 OP 
                indiceM+=16;
                indiceM+=(memoria[i]>>20)&0x0F;
                printf(" [%04d] %08X %3d %4s ",i,memoria[i],i+1,Mnemonicos[indiceM]);
            }
            else{                                                       //1 OP
                indiceM+=(memoria[i]>>24)&0x00F;
                printf(" [%04d] %08X %3d %4s ",i,memoria[i],i+1,Mnemonicos[indiceM]);
                imprimeOperando(memoria[i]>>22 & 0x03,memoria[i]&0xFFFF);
            }
        }
        else{//2 OP
            indiceM+=(memoria[i]>>28)&0x0F;
            printf(" [%04d] %08X %3d %4s ",i,memoria[i],i+1,Mnemonicos[indiceM]);
            imprimeOperando(memoria[i]>>26 & 0x03,memoria[i]>>12&0xFFF);
            printf("%4c ",',');
            imprimeOperando(memoria[i]>>24 & 0x03,memoria[i]&0xFFF);
        }
        printf("\n");
        i++;
    }
    printf("Registro \n");
    for(j=0;j<16;j++){
        if (strcmp(Procesador[j]," ")!=0)
            printf("%3s:  %8d",Procesador[j],registro[j]);
        else
            printf("              ");
        printf(" | ");
        }
    printf("\n");
}

void imprimeOperando(int tipoOp, int op){
    int tiporeg=op & 0x30;
    if (tipoOp==0){                                             //Inmediato
       printf("%2d",op);
    }
    else if (tipoOp==2)                                         //Directo
        printf("[%2d]",op);
    else if (tipoOp==1){                                        //Registro
        if (tiporeg==0)
            printf("E");
        printf ("%X",(op & 0x0F));   
        if (tiporeg==1)                                               // registro del 4to byte
            printf("L");
        else if (tiporeg==2)                                          // registro del 3 byte
            printf("H");
        else                                                          //registro de 2 bytes
            printf("X");
        }
    else{                                                       //Indirecto
        printf("[%x+ %d]",op & 0x0F,op& 0xFF0);
    }
}

void CreaDisco()
{   FILE* arch;
    int aux;
    arch=fopen("disco1.vdd","wb+");
    aux=0x56444430;//tipo de archivo
    fwrite(&aux,sizeof(aux),1,arch);
    aux=1;//numero de version
    fwrite(&aux,sizeof(aux),1,arch);
    aux=rand();//identificador del disco
    fwrite(&aux,sizeof(aux),1,arch);
    aux=rand();
    fwrite(&aux,sizeof(aux),1,arch);
    aux=rand();
    fwrite(&aux,sizeof(aux),1,arch);
    aux=rand();
    fwrite(&aux,sizeof(aux),1,arch);
    aux=0x0131F56E;//fecha creacion
    fwrite(&aux,sizeof(aux),1,arch);
    aux=0x08050301;//hora creacion
    fwrite(&aux,sizeof(aux),1,arch);
    aux=10;//tipo
    aux<<=8;
    aux|=128;//cantidad de cilindros
    aux<<=8;
    aux|=128;//cantidad de cabezas
    aux<<=8;
    aux|=128;//cantidad de sectores
    fwrite(&aux,sizeof(aux),1,arch);
    aux=512;//tamaño de un sector
    fwrite(&aux,sizeof(aux),1,arch);
    aux=0;//relleno
    fwrite(&aux,211,1,arch);
    fclose(arch);
}