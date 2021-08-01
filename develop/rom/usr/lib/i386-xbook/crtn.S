/* i386 C RunTime Library n, crtn.S */

.code32
.section .init
	add $12, %esp
	ret

.code32
.section .fini
	add $12, %esp
	ret
