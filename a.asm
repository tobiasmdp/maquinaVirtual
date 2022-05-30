ldh 2 ; Código del ES
ldl 64 ; offset del ES
mov EBX, AC ; Dirección del buffer de lectura

mov EDX, AC
SYS %3

mov AH, %03 ; escrib del disco
mov AL, 3 ; 3 sectores a leer
mov CH, 2 ; Cilindro 2
mov CL, 8 ; Cabeza 8
mov DH, 10 ; Sector 10
mov DL, 0 ; Unidad de disco 0
sys %D ; Indica que se realiza la escritura

mov AH, %02 ; Leer del disco
mov AL, 3 ; 3 sectores a leer
mov CH, 2 ; Cilindro 2
mov CL, 8 ; Cabeza 8
mov DH, 10 ; Sector 10
mov DL, 0 ; Unidad de disco 0
sys %D ; Indica que se realiza la lectura
sys %F
stop