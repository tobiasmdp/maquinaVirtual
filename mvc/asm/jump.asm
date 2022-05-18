            sys %F ;Jump Forest Jump
            mov     [1], 69 
            mov     [2], 110
            mov     [3], 113 
            mov     [4], 100 
            mov     [5], 115 
            mov     [6], 116 
            mov     [0], 1
            cmp     [0], 0
            jz      iguales
            jnz     distintos
            add     [1], 2
            jmp     sigue
            add     [5], 3
iguales:    add     [1], 5
            jmp     sigue
distintos:  add     [1], 1
sigue:      sub     [0], 2
            jp      positivo
            jn      negativo
            add     [6], 2
            jmp     otro
positivo:   add     [2], 3
            jmp     otro
negativo:   add     [2], 1
otro:       add     [0], 1
            jnp     nopos
            jnn     noneg
            jmp     otro2
            add     [3], 2
nopos:      add     [3], 1
            jmp     otro2
noneg:      add     [3], 3
            jmp     otro2
otro2:      add     [0], 1
            shr     [0], 1 
            jnn     noneg2
            jnp     nopos2
            add     [4], 5
            jmp     print
            add     [4], 3
nopos2:     add     [4], 2
            jmp     print
noneg2:     add     [4], 1
            jmp     print
print:      mov     eax, %010
            mov     ecx, 6
            mov     edx, 1
            sys     2
            stop



        