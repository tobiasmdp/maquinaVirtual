mov     EAX     ,   %FF
mov     ECX     ,   1
mov     EDX     ,   1
mov     [%1]    ,   'a 
unomas: sys     %2
add     [1]     ,   1
cmp     [1]     ,   'e'
jnp     unomas
mov AH, %00 ; Obtener los parámetros del disco
mov DL, 1 ; Unidad de disco 0
sys %D ; Indica que se realiza la lectura
stop 
