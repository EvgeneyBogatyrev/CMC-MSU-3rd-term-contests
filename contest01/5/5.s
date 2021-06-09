.section .data
output:
    .ascii "%d\n"
array:
    .long 0, -1, 5, 3, 1, 2, 3
amount:
    .long 2


.section .text
.globl main
.globl exchange
main:
    pushl   %ebp
    movl    %esp, %ebp
    subl    $8, %esp
    andl    $-16, %esp
    
    movl    $array, %eax
    movl    amount, %edx
    call    exchange

    movl    %ecx, 4(%esp)
    movl    $output, (%esp)
    call    printf    

    addl    $8, %esp
    movl    $0, %eax
    movl    %ebp, %esp
    popl    %ebp
    ret

exchange:
    pushl   %ebp
    movl    %esp, %ebp
    pushl   %ebx
    pushl   %esi
    subl    $16, %esp

    decl    %edx    
    leal    (%eax, %edx, 4), %ebx
    xorl    %ecx, %ecx


    .loop:
    cmp    %eax, %ebx
    jbe     .exit

    movl    (%eax), %edx
    movl    (%ebx), %esi

    cmp     %edx, %esi
    jae     .end

    mov     %esi, (%eax)
    mov     %edx, (%ebx)
    incl    %ecx

    .end:
    addl    $4, %eax
    subl    $4, %ebx
    jmp     .loop
    
    .exit:
    addl    $16, %esp
    popl    %esi
    popl    %ebx
    movl    %ebp, %esp
    popl    %ebp
    ret    
