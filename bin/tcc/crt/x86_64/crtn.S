/* x86-64 C RunTime Library n, crtn.S */

.section .init
	pop %rax
	ret

.section .fini
	pop %rax
	ret
