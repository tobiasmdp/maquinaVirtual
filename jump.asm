           base1 equ "hola"
            base2 equ "PAPA"
            base3 equ "CREo"
            ldh 0
            ldl 8
            mov     eax, %001
            mov     ecx, 10
            mov     edx, ac
            sys     3
            ldh 0
            ldl 8
            mov     eax, %001
            mov     ecx, 10
            mov     edx, ac
            sys     4
            ldh 2
            ldl 8
            mov eax, AC
            slen [EAX], [8]
            stop