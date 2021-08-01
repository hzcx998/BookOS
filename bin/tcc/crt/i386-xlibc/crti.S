/* i386 C RunTime Library i, crti.S */

.code32
.section .init
.global _init

_init:
	sub $12, %esp

.code32
.section ._fini
.global _fini

_fini:
	sub $12, %esp
