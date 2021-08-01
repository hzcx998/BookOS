/* ---------------------------------------------- */
/* alloca86_64.S */

.globl alloca

alloca:
    pop     %rdx
    mov     %rdi,%rax
    add     $15,%rax
    and     $-16,%rax
    jz      p3

    sub     %rax,%rsp
    mov     %rsp,%rax
p3:
    push    %rdx
    ret

/* ---------------------------------------------- */
