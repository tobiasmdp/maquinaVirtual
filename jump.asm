            base1 equ "hola"
            ldh 3
            ldl base1
            mov [2], AC
            slen [1], [2]
            sys %F ;Jump Forest Jump
            mov     [1], 69 
            mov     [2], 110
            mov     [3], 113 
            mov     [4], 100 
            mov     [5], 115 
            mov     [6], 116 
            ldh 3
            ldl 8
            mov     eax, %001
            mov     ecx, 1
            mov     edx, ac
            sys     3
            stop