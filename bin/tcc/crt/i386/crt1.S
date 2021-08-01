/* i386 C RunTime Library 1 (before crt0.S), crt1.S */

.text
.weak _init
.weak _fini
.weak _start

_start:
	xor %ebp, %ebp
	pop %ecx
	mov %esp, %eax
	and $-16, %esp
	push %esp
	push %esp
	push %edx
	push $_fini
	push $_init
	push %eax
	push %ecx
	push $main
	call __libc_start_main
