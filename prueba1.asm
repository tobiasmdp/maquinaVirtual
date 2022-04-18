Mov [9], 0
Mov [10], 0
Mov [11], 0
Mov [12], 0
Mov eax, %001
Mov ecx, 1
Mov edx, 9
Sys 1
Otro: Cmp 0, [9]
Jp  afuera
Add [11], [9]
Add [12], 1
Mov eax, %001
Mov ecx, 1
Mov edx, 9
Sys 1
Jmp otro
Afuera: div [11], [12]
Mov eax, %001
Mov ecx, 1
Mov edx, 11
Sys 2
stop