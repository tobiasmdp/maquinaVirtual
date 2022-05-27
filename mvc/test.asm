pepe1 EQU +33
pepe2 Equ -44
pepe3 equ "pepito"
pepestringsssss equ "peperoni"
texto1 EQU "josesito"
texto2 equ "narnia"
caracter equ 'c
                mov [EAX+caracter],22
otrosaltomas:   mov [EAX+texto2],22
                mov [EAX+pepe3],22
                mov [EAX+pepestring],22
                jmp otrosaltomas
                stop