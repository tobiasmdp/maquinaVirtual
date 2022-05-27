#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char* intToString(int numero);

int main(int argc, char const *argv[])
{   
    char str[200];

    FILE* archB;
    int instruccion;
    printf("   DECIMAL    |    HEXA\n");
    if ((archB = fopen(argv[2], "rb")) != NULL){
        fread(&instruccion,sizeof(instruccion), 1, archB);
        while (!feof(archB)){ 
            printf("%15d %02X %02X %02X %02X\n", instruccion,(instruccion>>24)&0xFF, (instruccion>>16)&0xFF, (instruccion>>8)&0xFF, (instruccion)&0xFF);
            fread(&instruccion,sizeof(int), 1, archB);
        }
    }
    fclose(archB);
    return 0;
}