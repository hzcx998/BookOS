/* ---------------------------------------------- */
/* alloca86_64.S */

.globl __bound_alloca
__bound_alloca:

    pop     %rdx
    mov     %rdi,%rax
    mov     %rax,%rsi   # size, a second parm to the __bound_new_region

    add     $15,%rax
    and     $-16,%rax
    jz      p3


    sub     %rax,%rsp
    mov     %rsp,%rdi   # pointer, a first parm to the __bound_new_region
    mov     %rsp,%rax

    push    %rdx
    push    %rax
    call   __bound_new_region
    pop     %rax
    pop     %rdx

p3:
    push    %rdx
    ret

/* ---------------------------------------------- */
