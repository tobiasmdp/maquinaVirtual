        mov     ax, %001
        mov     cx, 3
        mov     edx, 0
        sys     1 ; [0]^[1] % [2]
        mov     eax, [1]
        mov     ac, [0]
        mov     ecx, 32
rep:    sub     ecx, 1
        shl     eax, 1
        jp      rep
        sys     %F        
calc:   sub     ecx, 1
        jz      fin
        mov     eex, ac ;square
        mul     eex, eex 
        div     eex, [2]
        shl     eax, 1
        sys     %F 
        jnn     calc
        mov     eex, ac ;multiply
        mul     eex, [0]
        div     eex, [2]
        sys     %F 
        jmp     calc

fin:    sys     %F        
        mov     [4], ac
        mov     ax, %001
        mov     cx, 1
        mov     edx, 4
        sys     2    

