Mov EAX, %001
Mov ECX, 1
Mov EDX, 2
Sys 1
Mov eax, 1
For: CMP [2], 0
Jz afuera
MUL EAX, [2]
Sub [2], 1
Jmp for
Afuera: mov [6], EAX
Mov EAX, %001
Mov ECX, 1
Mov edx, 6
Sys 2
Stop
