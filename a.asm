ldh 2 ; Código del ES
ldl 64 ; offset del ES
mov EBX, AC ; Dirección del buffer de lectura
mov cx, 70
MOV AX, %000
mov EDX, AC
SYS %3
mov AH, %00 ; escrib del disco
mov AL, 3 ; 3 sectores a leer
mov CH, 130 ; Cilindro 0
mov CL, 1 ; Cabeza 1
mov DH, 2 ; Sector 2
mov DL, 0 ; Unidad de disco 0
sys %D ; Indica que se realiza la escritura

add EBX, 15

mov AH, %08 ; Leer del disco
mov AL, 3 ; 3 sectores a leer
mov CH, 130 ; Cilindro 0
mov CL, 1 ; Cabeza 1
mov DH, 2 ; Sector 2
mov DL, 0 ; Unidad de disco 0
sys %D ; Indica que se realiza la lectura

sys %F
stop