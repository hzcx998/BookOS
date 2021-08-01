/* x86-64 C RunTime Library 1 (before crt0.S), crt1.S */

.text
.weak _init
.weak _fini
.weak _start

_start:
	xor %rbp, %rbp
	mov %rdx, %r9
	pop %rsi
	mov %rsp, %rdx
	andq $-16, %rsp
	mov $_fini, %r8
	mov $_init, %rcx
	mov $main, %rdi
	call __libc_start_main
