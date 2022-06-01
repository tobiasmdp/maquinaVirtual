\\DATA 10
\\EXTRA 1000
\\STACK 1000
; estructura de nodo de árbol
value   equ     0
left    equ     1
right   equ     2
; constantes
NULL    equ     -1
titulo  equ  "--- PRUEBA ARBOL BINARIO DE BUSQUEDA ---"
msg_1   equ  "Recorrido en in-order:"
msg_2   equ  "Recorrido en pre-order:"
root    equ      1 

;main
main:           push    1       ;creo la raiz del árbol
                call    alloc
                add     sp, 1

                cmp     eax, NULL
                jz      main_end
                mov     [eax], NULL
                mov     [root], eax
                
                ldh     3
                ldl     titulo
                mov     edx, ac
                mov     ax, %800
                sys     %4

                push    10
                push    [root]
                call    insertValue
                add     sp, 2

                push    15
                push    [root]
                call    insertValue
                add     sp, 2

                push    5
                push    [root]
                call    insertValue
                add     sp, 2

                push    20
                push    [root]
                call    insertValue
                add     sp, 2

                push    12
                push    [root]
                call    insertValue
                add     sp, 2

                push    8
                push    [root]
                call    insertValue
                add     sp, 2

                push    3
                push    [root]
                call    insertValue
                add     sp, 2

                ldh     3
                ldl     msg_1
                mov     edx, ac
                mov     ax, %800
                sys     %4

                push    [root]
                call    print_inorder
                add     sp, 1

                ldh     3
                ldl     msg_2
                mov     edx, ac
                mov     ax, %800
                sys     %4

                push    [root]
                call    print_preorder
                add     sp, 1
main_end:       stop


; Insertar un valor en el árbol binario
; parámetros: 
;  - doble puntero a la raiz (bp+2)
;  - valor del nuevo nodo (bp+3)
insertValue:    push    bp
                mov     bp, sp
                push    eax

                
                push    [bp+3]
                call    newnode
                add     sp, 1

                cmp     eax, NULL
                jz      insertValue_end

                push    eax
                push    [bp+2]
                call    insertNode
                add     sp, 2

insertValue_end: pop    eax
                mov     bp, sp
                pop     bp
                ret



; Insertar un nuevo nodo en el árbol binario de búsqueda
; parametros: 
;  - doble puntero a la raiz (bp+2)
;  - puntero al nuevo nodo (bp+3)
insertNode:     push    bp
                mov     bp, sp
                push    ebx
                push    edx 

                
                mov     ebx, [bp+2]
                cmp     [ebx], NULL   ; si la raiz es NULL
                jz      insertNode_1  ; entonces insertar en la raiz

                mov     ebx, [ebx]    ; sino compara para ir por izquierda o derecha
                mov     edx, [bp+3]
                cmp     [edx+value], [ebx+value]
                jp      insertNode_right
                
insertNode_left: add     ebx, left
                push    edx
                push    ebx
                call    insertNode
                add     sp, 2

                jmp    insertNode_end

insertNode_right: add     ebx, right   
                push    edx
                push    ebx
                call    insertNode
                add     sp, 2

                jmp    insertNode_end

insertNode_1:   mov     [ebx], [bp+3]

insertNode_end: pop     edx
                pop     ebx                
                mov     bp, sp
                pop     bp
                ret                

; crear un nodo del árbol, se le pasa como parámetro el valor
; que se quiere guardar en el nodo
; devuelve el puntero al nuevo nodo en eax
newnode:        push    bp
                mov     bp, sp
                
                push    3
                call    alloc
                add     sp, 1

                cmp     eax, NULL
                jz      newnode_end

                mov     [eax+value], [bp+2]
                mov     [eax+left], NULL
                mov     [eax+right], NULL
newnode_end:    mov     bp, sp
                pop     bp
                ret

; alloc: devuelve en eax una dirección de memora con espacio para un nuevo nodo
alloc:          push    bp 
                mov     bp, sp  
                push    ac
                
                mov     eax, HP     ; asginamos la dirección de memoria 
                add     HP, [BP+2]  ; HP = HP + tamaño solicitado

                mov     ac, ES      
                shr     ac, 16
                ldh     %0002
                cmp     ac, HP      ; verifico que no se alla pasado del limite del extra segment
                jnn     alloc_end   ; si no se paso, finaliza            
                mov     HP, eax     ; si se pasó retrocede la asignación
                mov     eax, NULL   ; devuelve NULL            
alloc_end:      pop     ac
                mov     sp, bp
                pop     bp
                ret

; Imprimir un árbol binario en inorden
; parámetros:
;  - doble puntero a la raiz (bp+2)
print_inorder:  push    bp
                mov     bp, sp
                push    eax
                push    ebx
                push    ecx
                push    edx               
                
                mov     ebx, [bp+2]
                cmp     [ebx], NULL
                jz      print_inorder_end
                
                
                mov     eax, %0001
                mov     ecx, 1
                mov     edx, [ebx]

                mov     ebx, [ebx]

                add     ebx, 1 ;left
                push    ebx
                call    print_inorder
                add     sp, 1

                sys     %2 ;print

                add     ebx, 1 ;right
                push    ebx
                call    print_inorder
                add     sp, 1

print_inorder_end: pop     edx
                pop     ecx
                pop     ebx
                pop     eax
                mov     bp, sp
                pop     bp
                ret

; Imprimir un árbol binario en pre-order
; parámetros:
;  - doble puntero a la raiz (bp+2)
print_preorder:  push    bp
                mov     bp, sp
                push    eax
                push    ebx
                push    ecx
                push    edx 
                
                mov     ebx, [bp+2]
                cmp     [ebx], NULL
                jz      print_preorder_end
                
                mov     eax, %0001
                mov     ecx, 1
                mov     edx, [ebx]
                sys     %2 ;print

                mov     ebx, [ebx]

                add     ebx, 1 ;left
                push    ebx
                call    print_preorder
                add     sp, 1               

                add     ebx, 1 ;right
                push    ebx
                call    print_preorder
                add     sp, 1

print_preorder_end: pop     edx
                pop     ecx
                pop     ebx
                pop     eax
                mov     bp, sp
                pop     bp
                ret       
