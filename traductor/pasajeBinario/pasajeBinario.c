
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
    char operando[100];
    strcpy(operando, "@10");
    printf("%d",atoi("@10"));
    printf("%d",(int)("10"));

    return 0;
}