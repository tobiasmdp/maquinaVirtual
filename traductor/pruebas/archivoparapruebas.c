#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

const char* tablaMnemonicos[3][16] = {{"","STOP","","","","","","","","","","","","","",""}, 
                            {"SYS","JMP","JZ","JP","JN","JNZ","JNP","JNN","LDL","LDH","RND","NOT","","","",""},
                            {"MOV","ADD","SUB","SWAP","MUL","DIV","SHL","SUB","SHR","AND","OR","XOR","","","",""}};


int mnemonico_(char* cadena);

int main(int argc, char const *argv[])
{
    const char* mnem = {"JMP"};
    int a = mnemonico_((char*)mnem);
    printf("%d",a);
    printf("%d",a);
    // sizeof(tablaMnemonicos[0]) / sizeof(tablaMnemonicos[0][0])


}

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