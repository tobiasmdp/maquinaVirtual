add 22, [11]
mov [10], 0 ;inicializo variables
mov [20], 1
otro: cmp [20], 100 ;compara
jp fin ;salta si llegó a 100 o más
swap [10], [20]
mov eax, %001
mov ecx, 1
mov edx, 10
sys 2 ;print [10] en decimal
jmp otro
fin: stop
