global main
global exchange

extern printf

section .rodata
.LC0 db "%u", 10, 0

section .text
main:
    push    ebp
    mov     esp, ebp
    sub     esp, 8

    call    exchange

    mov     dword[esp], .LC0
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
    push    eip

    mov     ebx, eax
    lea     eip, [eax + 4 * edx - 4]
    xor     ecx, ecx

    cmp     ebx, eip
    jz      .exit
    
    .loop:
    mov     eax, dword[ebx]
    cmp     eax, dword[eip]
    jbz     .end

    mov     edx, dword[eip]
    mov     dword[ebx], edx
    mov     dword[eip], eax
    inc     ecx

    .end:
    add     ebx, 4
