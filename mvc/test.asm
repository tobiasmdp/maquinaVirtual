        pepe1 EQU 33
        pepe2 Equ -44

        ;
        ;Directo con simbolo
        mov [pepe1] ,22
        ;
        ;prueba de negativos
        mov [EAX+pepe1] ,22 ;positivo  33
        mov [EAX+pepe2] ,22 ;negativo  -44
        mov [EAX-pepe1] ,22 ;negativo  -33
        mov [EAX-pepe2] ,22 ;positivo  44
        mov [eax-+22] ,22; negativo -22
        mov [eax+-22] ,22; negativo -22, en hexa 
        mov [eax--22] ,22; positivo 22, en hexa 16
        ;
        ;indirectos
        mov     [EAx] ,22
        mov     [EAX+22]     ,   %FF 
        mov     [EAX+pepe1]     ,   %FF 
        mov     [EAX+pepe2]     ,   %FF 
        mov     [EAX+unomas]     ,   %FF
        mov     [EAX+asdasda]     ,   %FF ;este deberia tirar invalido, por asasas
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
