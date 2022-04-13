#include <stdio.h>
#include <string.h>

// 0xFFFFFFFF
//------------- headers -------------
int mnemonico(char lineaParseada[]);
//------------- funciones -------------
int mnemonico(char lineaParseada[])
{
    int instruccion;
    if (strcmp(lineaParseada, "MOV") == 0)
        instruccion = 0x00000000;
    else if (strcmp(lineaParseada, "ADD") == 0)
        instruccion = 0x10000000;
    else if (strcmp(lineaParseada, "SUB") == 0)
        instruccion = 0x20000000;
    else if (strcmp(lineaParseada, "SWAP") == 0)
        instruccion = 0x30000000;
    else if (strcmp(lineaParseada, "MUL") == 0)
        instruccion = 0x40000000;
    else if (strcmp(lineaParseada, "DIV") == 0)
        instruccion = 0x50000000;
    else if (strcmp(lineaParseada, "CMP") == 0)
        instruccion = 0x60000000;
    else if (strcmp(lineaParseada, "SHL") == 0)
        instruccion = 0x70000000;
    else if (strcmp(lineaParseada, "SHR") == 0)
        instruccion = 0x80000000;
    else if (strcmp(lineaParseada, "AND") == 0)
        instruccion = 0x90000000;
    else if (strcmp(lineaParseada, "OR") == 0)
        instruccion = 0xA0000000;
    else if (strcmp(lineaParseada, "XOR") == 0)
        instruccion = 0xB0000000;
    // ---------------- 1 Operando ----------------
    else if (strcmp(lineaParseada, "SYS") == 0)
        instruccion = 0xF0000000;
    else if (strcmp(lineaParseada, "JMP") == 0)
        instruccion = 0xF1000000;
    else if (strcmp(lineaParseada, "JZ") == 0)
        instruccion = 0xF2000000;
    else if (strcmp(lineaParseada, "JP") == 0)
        instruccion = 0xF3000000;
    else if (strcmp(lineaParseada, "JN") == 0)
        instruccion = 0xF4000000;
    else if (strcmp(lineaParseada, "JNZ") == 0)
        instruccion = 0xF5000000;
    else if (strcmp(lineaParseada, "JNP") == 0)
        instruccion = 0xF6000000;
    else if (strcmp(lineaParseada, "JNN") == 0)
        instruccion = 0xF7000000;
    else if (strcmp(lineaParseada, "LDL") == 0)
        instruccion = 0xF8000000;
    else if (strcmp(lineaParseada, "LDH") == 0)
        instruccion = 0xF9000000;
    else if (strcmp(lineaParseada, "RND") == 0)
        instruccion = 0xFA000000;
    else if (strcmp(lineaParseada, "NOT") == 0)
        instruccion = 0xFB000000;
    // ---------------- 0 Operandos ----------------
    else if (strcmp(lineaParseada, "STOP") == 0)
        instruccion = 0xFF100000;
    // ---------------- Operando erroneo ----------------
    else
        instruccion = 0xF;
    return instruccion;
}

int main(int argc, char const *argv[])
{
    int instruccion = 0xFFFFFFFF;
    printf("%d %08X %08o\n", instruccion, instruccion, instruccion);
    instruccion = (instruccion & (0x10000000));
    printf("%d %08X %08o\n", instruccion, instruccion, instruccion);

    char lineaParseada[10] = "asd";
    instruccion = mnemonico(lineaParseada);

    printf("%d %08X %08o\n", instruccion, instruccion, instruccion);
    return 0;
}
