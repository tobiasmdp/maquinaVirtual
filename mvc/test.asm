        mov     EAX     ,   %FF
        mov     ECX     ,   1
        mov     EDX     ,   1
        mov     [%1]    ,   'a 
unomas: sys     %2
        add     [1]     ,   1
        cmp     [1]     ,   'e'
        jnp     unomas
        stop
