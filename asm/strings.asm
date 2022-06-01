\\EXTRA 15
;Inicializa HEAP (manual)
        ldh     2
        ldl     0
        mov     [ac], ac
        add     [ac], 1
; Escribe mensaje al usuario
msg     equ     "Escriba palabras seguidas de ENTER (en blanco para terminar)"
        ldh     3
        ldl     msg
        mov     eax, %800
        mov     ecx, 1000  
        mov     edx, ac
        sys     %4
        mov     ecx, 1 
; Lee una palabra en DS[0]
        ldh     2
        ldl     0
ini:    mov     eax, %900
        mov     ecx, 1000
        mov     edx, 0
        sys     %3
        slen    ecx, [0]
        cmp     ecx, 0           ;Si está vacía... 
        jz      finlee          ;...termina la lectura 
        mov     edx, [ac]        ;Sino, muevo el HEAP a edx        
        add     [ac], ecx      ;Incremento el HEAP para reservar la memoria 
        add     [ac], 1       ;Uno más por el \0
        smov    [edx], [0]       ;Agrega la palabra en el ES 
        jmp     ini
finlee: ldl     1
        mov edx, ac              ;Inicializa 1 para comenzar el recorrido 
        ldl     0
        mov ecx, [ac]          
        sub ecx, 1               ;Posiciona a ecx en el último \0
next:   cmp edx, ecx              ;Termina cuando edx llega al último \0
        jz  fin
may:    and [edx], %DF        ;Pasa a MAYUSCULAS
        cmp [edx], 0          ;Si encuentra un \0 ...
        jnz sig         
        mov [edx], %20        ;... pone un espacio para contactenar 
sig:    add edx, 1               
        jmp next
; Muestra cadena concatenada y pasada a mayusculas
fin:    SYS %F ;mostrar el ES
        ldl     1
        ldh     2
        slen    ecx, [ac]
        add     ecx, 1
        mov     edx, ac
        mov     eax, %019
        sys     %2 
        stop 


        



