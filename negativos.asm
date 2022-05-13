        ldh   %FFFFF
        ldl   %01FF
        mov   ebx, ac   ; EBX = %FFFF01FF
        mov   [0], BL   ; -1
        mov   [1], BH   ;  1
        sub   BH, 2
        mov   [2], BH   
        mul   ebx, -1
        mov   [3], ebx  
        not   ebx
        add   ebx, 1
        shr   ebx, 16        
        mov   [4], ebx  
        mul   bh, bl 
        mov   [5], bh   
        mov   [6], bx   
        ldh   %7fff
        ldl   %ffff
        mov   [7], ac   ; [7] = %7FFFFFFF
        mov   [8], ac   
        add   [8], 1    
        mov   [9], [8]
        not   [9]       ;  2.147.483.647  
        mov   eax, %009
        mov   ecx, 10
        mov   edx, 0 
        sys   2                