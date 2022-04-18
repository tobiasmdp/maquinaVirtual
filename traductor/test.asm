jmp lee
xor EBX, EBX 
lee: mov EDX, 1 
mov ECX, 1 
mov EAX, %001 
sys 1 
cmp 1, [1] 
jz sigue 
cmp 0, [1] 
jnz fin 
sigue: shl EBX, 1
add EBX, [1] 
jmp lee 
fin: mov [2], EBX 
mov EAX, %001 
mov ECX, 1 
mov EDX, 2 
sys 2 
stop