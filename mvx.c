#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <conio.h>

#define TM 4096 //tamaño memoria
#define TR 16 //tamaño registro
#define DS 0  //accesos directos de los registros importantes
#define IP 5
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

void LeeArch(int[],int[],int*);
void ExtraerUnOperando(int ,int**,int*, int[] ,int [],int *);
void ExtraerDosOperandos(int ,int** ,int** ,int*,int *, int[] ,int [],int *,int *);

void MOV(int *,int ,int *,int ,int ,int ,int [],int []);
void ADD(int *,int ,int *,int ,int ,int ,int [],int []);
void SUB(int *,int ,int *,int ,int ,int ,int [],int []);
void MUL(int *,int ,int *,int ,int ,int ,int [],int []);
void DIV(int *,int ,int *,int ,int ,int ,int [],int []);
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
void STOP(int *,int ,int *,int ,int ,int ,int [],int []);

int get_value(int *,int );
void set_value(int *,int , int);
void registroCC(int, int[]);
int checkFlag(char []);
void breakpoint(int [], int []);
void dividenum(char [],int *, int *);
void disassembler(int [],int []);

typedef void T_fun(int *,int ,int *,int ,int ,int ,int [],int []);//tipo de funcion

/*----------------------------------------------------Comienzo del programa-----------------------------------------------*/

int p=0; /*si aparece p en el sys se cambia*/
int pri=0;
int main(int argc, char const *argv[]){ // VER BIEN LOS ARGUMENTOS
    int indicef,inst; //indicef: numero de operador aux: Almacena la instruccion a ejecutar
    int *A,*B,C,D,mascaraA,mascaraB,valido=1;
    int memoria[TM],registro[TR]={0};
    T_fun* ArrayFunc[]={MOV,ADD,SUB,SWAP,MUL,DIV,CMP,SHL,SHR,AND,OR,XOR,0,0,0,0,SYS,JMP,JZ,JP,JN,JNZ,JNP,JNN,LDL,LDH,RND,NOT,0,0,0,0,0,STOP}; //Arreglo de punteros a funciones, cuando las tengamos las metemos ahi  
    
    if (checkFlag("-c"))  /*limpio la pantalla al inicio si -c esta como argumento*/
        system("cls");
    LeeArch(memoria,registro,&valido);
    if (checkFlag("-d"))
        disassembler(registro,memoria);
    while (registro[IP]<registro[DS] && valido){
        indicef=0; 
        inst=memoria[registro[IP]];
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
        (*ArrayFunc[indicef])(A,mascaraA,B,C,D,mascaraB,memoria,registro); //FUNCIONAL
        if (checkFlag("-b") && p==1 && pri>1)
            breakpoint(registro,memoria);
    }     
    return 0;
}

void LeeArch(int memoria[],int registro[],int *valido){
    FILE* arch;
    arch=fopen(_argv[1],"rb");
    int header[6],i=0;
    if (arch == NULL)
        printf("Error en la apertura. Es posible que el archivo no exista");

    fread(header+i,sizeof(int),1,arch);
    while(!feof(arch) && i<5){
        i++;
        fread(header+i,sizeof(int),1,arch);
    }
    i=0;
    fread(memoria+i,sizeof(int),1,arch);
    while(!feof(arch)){
        i++;
        fread(memoria+i,sizeof(int),1,arch);
    }
    registro[DS]=i;

   if((header[0]>>24&0x0FF)!='M' || (header[0]>>16&0x0FF)!='V' || (header[0]>>8&0x0FF)!='-' || (header[0]&0x0FF)!='1' || registro[DS]!=header[1] || (header[5]>>24&0x0FF)!='V' || (header[5]>>16&0x0FF)!='.' || (header[5]>>8&0x0FF)!='2' || (header[5]&0x0FF)!='2') {
       *valido=0;
        printf("El archivo no pudo ser validado");
    } 
    else{
        printf("El archivo fue validado\n");
    }

    fclose(arch);
}

/*-----------------------------------------------------Extraccion operandos------------------------------------------------------*/

void ExtraerUnOperando(int inst,int **A,int *C, int memoria[],int registro[],int *mascaraA){
    if ((inst>>22 & 0x03)==0){//Inmediato
        *C=inst & 0x0FFFF;
        *C<<=20; 
        *C>>=20;
        *A=C;
   }
    else
        if ((inst>>22 & 0x03)==2)//Directo
            *A=&memoria[registro[DS]+(inst & 0x0FFFF)];
        else{//Registro
           *A=&registro[(inst & 0x0F)];
           if ((inst>>4 & 0x03)==1) // registro del 4to byte
             *mascaraA=LOW_MASK;
           else
                 if ((inst>>4 & 0x03)==2) // registro del 3 byte
                   *mascaraA=HIGH_MASK;
                else //registro de 2 bytes
                  if ((inst>>4 & 0x03)==3)
                    *mascaraA= REG_MASK;
                 //registro de 4 bytes (sale automatico)
        }
}

void ExtraerDosOperandos(int inst,int **A,int **B,int *C, int *D, int memoria[],int registro[],int *mascaraA, int *mascaraB){
    //primer operando
    *mascaraA=*mascaraB=EXTENDED_MASK;
    if ((inst>>26 & 0x03)==0){//Inmediato
        *C=inst>>12 & 0x0FFF;
        *C<<=20; 
        *C>>=20;
        *A=C;
   }
    else
        if ((inst>>26 & 0x03)==2)//Directo
            *A=&memoria[registro[DS]+(inst>>12 & 0x0FFF)];
        else{//Registro
           *A=&registro[(inst>>12 & 0x0F)];
           if ((inst>>16 & 0x03)==1) // registro del 4to byte
             *mascaraA=LOW_MASK;
           else
                 if ((inst>>16 & 0x03)==2) // registro del 3 byte
                   *mascaraA=HIGH_MASK;
                else //registro de 2 bytes
                  if ((inst>>16 & 0x03)==3)
                    *mascaraA= REG_MASK;
                 //registro de 4 bytes (sale automatico)
        }
    //segundo operando
    if ((inst>>24 & 0x03)==0){//Inmediato
        *D=inst & 0x0FFF;
        *D<<=20; 
        *D>>=20;
        *B=D; // tambien analizar
   }
    else
        if ((inst>>24 & 0x03)==2)//Directo
            *B=&memoria[registro[DS]+(inst & 0x0FFF)];
        else{//Registro
           *B=&registro[(inst & 0x0F)];
           if ((inst>>4 & 0x03)==1) // registro del 4to byte
             *mascaraB=LOW_MASK;
           else
                 if ((inst>>4 & 0x03)==2) // registro del 3 byte
                   *mascaraB=HIGH_MASK;
                else //registro de 2 bytes
                  if ((inst>>4 & 0x03)==3)
                    *mascaraB= REG_MASK;
                 //registro de 4 bytes (sale automatico)
        }
}

/*--------------------------------------------------Funciones de 2 operandos:-------------------------------------------------*/

void MOV(int *A,int mascaraA,int *B,int C,int D,int mascaraB,int memoria[],int registro[]){
    set_value (A,get_value(B,mascaraB),mascaraA);
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
    registroCC(auxA%auxB,registro);
    set_value (A,auxA/auxB,mascaraA);
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
    int indice,contc,cont=0,longitud;
    char caracter[]={};
    if ((*A & mascaraA) == 1){// lectura // revisar contadores
        indice=registro[EDX];
        while (cont<(registro[ECX]&REG_MASK)){
            if (( registro[EAX]&0x0800)>>11==0)
                printf("[%04d]: ",(indice+cont));
            if ((registro[EAX]&0x0100)>>8==0){
                if ((registro[EAX]&0x001)==1)
                    scanf("%d",memoria+(registro[DS]+indice+cont));
                if ((registro[EAX]&0x004)==1)
                    scanf("%o",memoria+(registro[DS]+indice+cont));
                if ((registro[EAX]&0x008)==1)
                    scanf("%X",memoria+(registro[DS]+indice+cont));
                cont++;
            }
            else {
                scanf("%s",caracter);
                longitud=strlen(caracter);
                contc=0;
                while (contc<longitud && cont<(registro[ECX]&REG_MASK)){
                    memoria[registro[DS]+indice+cont]=caracter[contc];
                    contc++;
                    cont++;
                }
            }
        }
    }
    else 
      if ((*A & mascaraA) == 2){ // escritura
        indice=registro[EDX];
        cont=0;
        while (cont<(registro[ECX]&REG_MASK)){
            if ((registro[EAX]&0x0800)>>11==0)
                printf("[%04d]: ",(indice+cont));
            if ((registro[EAX]&0x001)==1)
                printf("%d ",memoria[registro[DS]+indice+cont]);
            if ((registro[EAX]&0x004)==1)
                printf("%o ",memoria[registro[DS]+indice+cont]);
            if ((registro[EAX]&0x008)==1)
                printf("%X ",memoria[registro[DS]+indice+cont]);
            if ((registro[EAX]&0x010)>>4==1)
                if ((memoria[registro[DS]+indice+cont]&0x0FF<=31) || (memoria[registro[DS]+indice+cont]&0x0FF==127))
                    printf(".");
                else
                    printf("%c ",(memoria[registro[DS]+indice+cont]&0x0FF));
            if ((registro[EAX]&0x0100)>>8==0)
                printf("\n");
            cont++;
        }
      }
      else
        if (*A & mascaraA == 15){ //breakpoint - flags
            if (checkFlag("-c"))
                system("cls");
            if (checkFlag("-d")){
                disassembler(registro,memoria);
            if (checkFlag("-b"))
                breakpoint(registro,memoria);
            }
        }
}

void JMP(int *A,int mascaraA,int *B,int C,int D,int mascaraB,int memoria[],int registro[]){
    registro[IP]=get_value(A,mascaraA); // mueve el ip al valor del operando
}

void JZ(int *A,int mascaraA,int *B,int C,int D,int mascaraB,int memoria[],int registro[]){
    if ((registro[CC]&0x01)==1)
        JMP(A,mascaraA,B,C,D,mascaraB,memoria,registro);
}

void JP(int *A,int mascaraA,int *B,int C,int D,int mascaraB,int memoria[],int registro[]){ //ver bien estas condiciones
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
    auxA<<=30;
    set_value (&registro[AC],auxA,0xC0000000);
}

void LDL(int *A,int mascaraA,int *B,int C,int D,int mascaraB,int memoria[],int registro[]){
    int auxA=get_value(A,mascaraA);
    set_value (&registro[AC],auxA,0x03);
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

/*Funciones de 0 operandos:*/

void STOP(int *A,int mascaraA,int *B,int C,int D,int mascaraB,int memoria[],int registro[]){
    registro[IP]=registro[DS];
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


int checkFlag(char bandera []){ //ojo aca con el paso de parametros
    int i=0;
    while (i<_argc && strcmp(_argv[i],bandera)!=0)
        i++;
    return (i==_argc)?0:1;
}

void breakpoint(int registro[], int memoria[]){
char sig[10];
int aux,aux1;
    pri++;
    printf("[%04d] cmd: ",registro[IP]-1);
    fgets(sig,10,stdin);
    while (sig[0]!='r' && sig[0]!='p'){
        dividenum(sig,&aux,&aux1);
        if (aux1==-1)
          printf("[%04d] %X %d \n",aux,memoria[aux],memoria[aux]);
        else
          for (int i=aux;i<=aux1;i++)
            printf("[%04d] %X %d \n",i,memoria[i],memoria[i]);
        printf("[%04d] cmd: ",registro[IP]-1);
        fgets(sig,10,stdin);
    }
    if (sig[0]=='p')
      p=1;
    else
      p=0;
}

void dividenum(char sig[],int *aux, int *aux1){
    char cadaux[5],cadaux1[5]="-1";
    int i=0,maxcad=strlen(sig);
    while (i<=maxcad && sig[i]!=' '){
     cadaux[i]=sig[i];
     i++;
     }
     if (i<=maxcad && sig[i]==' '){
         while (i<=maxcad){
          cadaux1[i-5]=sig[i];
          i++;
        }
     }
    *aux=atoi(cadaux);
    *aux1=atoi(cadaux1);
}

void disassembler(int registro[],int memoria[]){ // ver impresion dessassembler
char* Mnemonicos[34]= {"MOV","ADD","SUB","SWAP","MUL","DIV","CMP","SHL","SHR","AND","OR","XOR","0","0","0","0","SYS","JMP","JZ","JP","JN","JNZ","JNP","JNN","LDL","LDH","RND","NOT","0","0","0","0","0","STOP"};
char* Procesador[16]={"DS"," "," "," "," ","IP"," "," ","CC","AC","EAX","EBX","ECX","EDX","EEX","EFX"};
int indiceM,indiceP,j,i=registro[IP];
    printf("Codigo: \n");
    while (i<registro[DS] && i<10+registro[IP]){
        if (i==registro[IP])
            printf(">");
        else
            printf(" ");
        indiceM=0;
        if ((memoria[i]&0xF0000000)==0xF0000000){
            indiceM+=16;
            if((memoria[i]&0x0F000000)==0x0F000000){//0 OP 
                indiceM+=16;
                indiceM+=(memoria[i]>>20)&0x0F;
                printf(" [%04d] %08X %3d %4s ",i,memoria[i],i+1,Mnemonicos[indiceM]);
            }
            else{//1 OP
                indiceM+=(memoria[i]>>24)&0x00F;
                printf(" [%04d] %08X %3d %4s ",i,memoria[i],i+1,Mnemonicos[indiceM]);
                  if ((memoria[i]>>22 & 0x03)==0)//Inmediato
                    printf("%3d",(memoria[i]&0x0FFFF));
                  else
                    if ((memoria[i]>>22 & 0x03)==2)//Directo
                      printf("[%2d]",(memoria[i] & 0x0FFFF));
                    else{//Registro
                      if ((memoria[i]>>4 & 0x03)==0)
                        printf("E");
                      printf ("%X",(memoria[i] & 0x0F));   
                        if ((memoria[i]>>4 & 0x03)==1) // registro del 4to byte
                          printf("L");
                        else
                          if ((memoria[i]>>4 & 0x03)==2) // registro del 3 byte
                            printf("H");
                          else //registro de 2 bytes
                            printf("X");
                    }
            }
        }
        else{//2 OP
            indiceM+=(memoria[i]>>28)&0x0F;
            printf(" [%04d] %08X %3d %4s ",i,memoria[i],i+1,Mnemonicos[indiceM]);
                  if ((memoria[i]>>26 & 0x03)==0)//Inmediato primer operando
                    printf("%3d",((memoria[i]>>12)&0x0FFF));
                  else
                    if ((memoria[i]>>26 & 0x03)==2)//Directo
                      printf("[%2d]",(memoria[i]>>12) & 0x0FFF);//aca hay algo
                    else{//Registro
                      if ((memoria[i]>>16 & 0x03)==0)
                        printf("E");
                      printf ("%X",(memoria[i]>>12 & 0x0F));   
                        if ((memoria[i]>>16 & 0x03)==1) // registro del 4to byte
                          printf("L");
                        else
                          if ((memoria[i]>>16 & 0x03)==2) // registro del 3 byte
                              printf("H");
                          else //registro de 2 bytes
                              printf("X");
                      printf(" ");
                    }
                    printf(", ");
                    if ((memoria[i]>>24 & 0x03)==0)//Inmediato segudo operando
                      printf("%3d",((memoria[i])&0x0FFF));
                    else
                      if ((memoria[i]>>24 & 0x03)==2)//Directo
                        printf("[%2d]",(memoria[i] & 0x0FFF));
                      else{//Registro
                        if ((memoria[i]>>4 & 0x03)==0)
                          printf("E");
                        printf ("%X",(memoria[i] & 0x0F));   
                        if ((memoria[i]>>4 & 0x03)==1) // registro del 4to byte
                          printf("L");
                        else
                            if ((memoria[i]>>4 & 0x03)==2) // registro del 3 byte
                              printf("H");
                            else //registro de 2 bytes
                              printf("X");
                      
                    }
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