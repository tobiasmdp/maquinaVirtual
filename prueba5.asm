Mov eax, %001
Mov ecx, 1
Mov edx, 1 
Sys 1
Mov [5], 2
Mov [7], 0
Mov [8], 1
For: Cmp [1], [5]
Jz afuera 
Mov [2], [1]
Div [2], [5]
Cmp AC, 0
Jz afuera
Add [5], 1
Jmp for
Afuera: cmp [1], [5]
Jz noes
Mov eax, %001
Mov ecx, 1
Mov edx, 8
Jmp fin
noes: Mov eax, %001
Mov ecx, 1
Mov edx, 7
Sys 2 
Fin: stop
