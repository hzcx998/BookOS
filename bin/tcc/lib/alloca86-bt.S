/* ---------------------------------------------- */
/* alloca86-bt.S */

.globl __bound_alloca

__bound_alloca:
    pop     %edx
    pop     %eax
    mov     %eax, %ecx
    add     $3,%eax
    and     $-4,%eax
    jz      p6

    sub     %eax,%esp
    mov     %esp,%eax

    push    %edx
    push    %eax
    push    %ecx
    push    %eax
    call   __bound_new_region
    add    $8, %esp
    pop     %eax
    pop     %edx

p6:
    push    %edx
    push    %edx
    ret

/* mark stack as nonexecutable */
#if defined __ELF__ && defined __linux__
    .section    .note.GNU-stack,"",@progbits
#endif
/* ---------------------------------------------- */
