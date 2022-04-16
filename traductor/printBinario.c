#include <stdio.h>
#include <string.h>

int main(int argc, char const *argv[])
{
    FILE* archB;
    int instruccion;
    printf("   DECIMAL    |    HEXA\n");
    if ((archB = fopen(argv[2], "r")) != NULL){
        fread(&instruccion,sizeof(instruccion), 1, archB);
        //while (!feof(archB)){ 
        for(int i=0; i<22;i++){
            printf("%15d %02X %02X %02X %02X\n", instruccion,(instruccion>>24)&0xFF, (instruccion>>16)&0xFF, (instruccion>>8)&0xFF, (instruccion)&0xFF);
            fread(&instruccion,sizeof(int), 1, archB);
        }
    }
    fclose(archB);
    return 0;
}
