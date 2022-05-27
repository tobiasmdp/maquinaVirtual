        pepe1 EQU 33
        pepe2 Equ -44

        jnp     unomas
        ;
        ;Directo con simbolo
        mov [pepe1] ,22
        ;
        ;pueba de indirectos 
        mov [HP] ,22 ;positivo  33 ----- 0C 00 40 16
        mov [AX] ,22 ;positivo  33 ----- 0C 00 A0 16
        mov [IP] ,22 ;positivo  33 ----- 0C 00 50 16
        mov [SP+22] ,22 ;positivo  33 ----- 0C 16 60 16
        mov [BP+pepe1] ,22 ;positivo  33 ----- 0C 21 70 16
        mov [AC-pepe1] ,22 ;positivo  33 ----- 0C DF 90 16
        mov [SP-22] ,22 ;positivo  33 ----- 0C EA 60 16

        ;prueba de negativos e indirectos
        mov [EAX+pepe1] ,22 ;positivo  33 ----- 0C 21 A0 16
        mov [EAX+pepe2] ,22 ;negativo  -44 ----- 0C D4 A0 16
        mov [EAX-pepe1] ,22 ;negativo  -33 ----- 0C DF A0 16
        mov [EAX-pepe2] ,22 ;positivo  44 ----- 0C 2C A0 16
        mov [eax-22] ,22; negativo -22 ----- 0C EA A0 16
        mov [eax+-22] ,22; negativo -22, en hexa ----- 
        mov [eax--22] ,22; positivo 22, en hexa 16 ----- 

        ;
        ;formato de simbolo valido, pero inexistente
        mov     [asdasd] , 2 
        mov     [as+dasd] , 2
        mov     [EA+dasd] , 2
        mov     [EAD+dasd] , 2
        ;
        ;simbolos existentes
        mov     [unomas] , 2
        mov     2 ,     [unomas]
        mov     unomas unomas
        stop
        ;
        ;
unomas: sys     %2
        add     [1]     ,   1
        cmp     [1]     ,   'e'
        jnp     unomas
        stop
