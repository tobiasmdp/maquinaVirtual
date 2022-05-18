        
        \\DATA 10
        \\EXTRA 3000
        \\STACK 5000
        ;\\STACK 8000 ;error memoria
        
;descomentar lo siguiente cuando esten los simbolos implementados
        ;BASE0 EQU #16
        ;BASE0 EQU %22 error base duplicada
        ;BASE1 EQU %22
        ;BASE2 EQU @22 
        ;BASE3 EQU 12
        ;BASE4 EQU 'A ;estos tambien los tiene q aceptar

        PUSH    ECX
        PUSH    [1]
PEPE:   PUSH    %2 
        PUSH    [EAX]
        PUSH    [EAX+22] ;siempre va en base 10
        PUSH    [EAX+BASE] ;preguntar si el simbolo puede tener numeros en el nombre

;la idea es reemplazar el PUSH,
;por cada uno de estos mnemonicos
    ;2 operandos         POP
    ;2 operandos         CALL    
    ;1 operandos         SLEN    
    ;1 operandos         SMOV    
    ;1 operandos         SCMP    
    ;0 operandos         RET     


        jnp     unomas ;este esta antes de la linea que se llama unomas
        JNP     pEpe

;a partir de aca es un stock, las pruebas forzadas eran lo de arriba
        mov     ECX     ,   1
        mov     EDX     ,   1
        mov     [%1]    ,   'a 
unomas: sys     %2
        add     [1]     ,   1
        cmp     [1]     ,   'e'

        stop
