/* i386 C RunTime Library 1, crt1.S */

.weak __libc_start_main
.weak __libc_csu_fini
.weak __libc_csu_init
.weak main

.code32
.section .text

.global _start
_start:
	xor %ebp, %ebp
	push %esp
	push $__libc_csu_fini
	push $__libc_csu_init
	# put arg on stack
	push %edx
	push %ebx
	push %ecx
	push $main
	call __libc_start_main
	# should never be here.
	hlt
