\\EXTRA 5000
ldh 2
ldl 64
mov EBX, AC ; Dirección del buffer de lectura
mov AH, %00 ; Leer del disco
mov AL, 3 ; 3 sectores a leer
mov CH, 2 ; Cilindro 2
mov CL, 8 ; Cabeza 8
mov DH, 10 ; Sector 10
mov DL, 2 ; Unidad de disco 0
sys %D 
stop 