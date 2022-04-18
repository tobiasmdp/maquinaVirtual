Mov eax, %001
Mov ecx, 1
Mov edx, 5
Sys 1
Mov eax, %001
Mov ecx, 1
Mov edx, 3
Sys 1
And [5], [1]
Mov [1], 1
Mov [2], 0
Otro cmp [1], 32
Jz afuera
Mov [4], [5]
And [4], 1
Cmp [4], 1
Jnz noentra
Add [2], 1
Noentra: Shr [5], 1
Add [1], 1
jmp otro
afuera: Mov eax, %001
Mov ecx, 1
Mov edx, 2
Sys 2 
