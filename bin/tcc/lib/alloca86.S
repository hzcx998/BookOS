/* ---------------------------------------------- */
/* alloca86.S */

.globl alloca

alloca:
    pop     %edx
    pop     %eax
    add     $3,%eax
    and     $-4,%eax
    jz      p3

    sub     %eax,%esp
    mov     %esp,%eax
p3:
    push    %edx
    push    %edx
    ret

/* ---------------------------------------------- */
