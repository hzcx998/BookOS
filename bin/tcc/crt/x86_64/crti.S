/* x86-64 C RunTime Library i, crti.S */

.section .init
.global _init

_init:
	push %rax

.section .fini
.global _fini

_fini:
	push %rax
