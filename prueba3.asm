Mov EAX, %001
Mov ECX, 1
Mov EDX, 2
Sys 1
Mov EAX, %001
Mov ECX, 1
Mov EDX, 3
Sys 1
Mov BX, [2]
MOV CX, [3]
Mov [5], 0
Mov eax, 0
For: CMP [3], [5]
Jz afuera
Add [AX], [2]
Add [5], 1
Jmp for
afuera: mov [6], EAX
Mov EAX, %001
Mov ECX, 1
Mov edx, 6
Sys 2
stop
