/* i386 C RunTime Library n, crtn.S */

.section .init
	add $12, %esp
	ret

.section .fini
	add $12, %esp
	ret
