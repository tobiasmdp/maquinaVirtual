        pepe1 EQU 33
        pepe2 Equ 44

        mov     [EAX+22]     ,   %FF ;esto no es un simbolo
        mov     [EAX+pepe1]     ,   %FF ;esto no es un simbolo
        mov     [EAX+pepe2]     ,   %FF ;esto no es un simbolo
        
        mov     [asdasd] , 2 ;esto creo q si es un simbolo
        mov     [as+dasd] , 2 ;esto creo q si es un simbolo
        mov     [EA+dasd] , 2 ;esto creo q si es un simbolo

        ;sadsdasd
unomas: sys     %2
        add     [1]     ,   1
        cmp     [1]     ,   'e'
        jnp     unomas
        stop
