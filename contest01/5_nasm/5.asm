global main
global exchange

extern printf

section .rodata
LC0 db "%u", 10, 0

section .data
a dd 5, 1, 2, 3

section .text
main:
    push    ebp
    mov     esp, ebp
    sub     esp, 8

    mov     eax, a
    mov     edx, 4
    call    exchange

    mov     dword[esp], LC0
    mov     dword[esp + 4], ecx
    call    printf

    xor     eax, eax
    add     esp, 8
    pop     ebp
    ret

exchange:
    push    ebp
    mov     esp, ebp
    push    ebx
    push    esi

    mov     ebx, eax
    lea     esi, [eax + 4 * edx - 4]
    xor     ecx, ecx

    cmp     ebx, esi
    jz      .exit
    
    .loop:
    mov     eax, dword[ebx]
    cmp     eax, dword[esi]

    jbe     .end

    mov     edx, dword[esi]
    mov     dword[ebx], edx
    mov     dword[esi], eax
    inc     ecx

    .end:
    add     ebx, 4
    sub     esi, 4
    cmp     ebx, esi
    jb      .loop

    .exit:
    pop     esi
    pop     ebx
    pop     ebp
    ret
