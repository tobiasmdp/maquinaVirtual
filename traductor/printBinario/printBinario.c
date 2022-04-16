#include <stdio.h>
#include <string.h>

int main(int argc, char const *argv[])
{
    FILE* archB;
    int instruccion;
    if ((archB = fopen(argv[2], "r")) != NULL){
        while (fread(&instruccion,sizeof(instruccion)) != NULL){ 
            
printf("%0d %02X %02X %02X %02X")
        }
    }
    return 0;
}
