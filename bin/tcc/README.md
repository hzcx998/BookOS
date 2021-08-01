# Path of TCC
## install:
	/usr/local/lib/tcc
## include:
	/usr/local/lib/tcc/include
	/usr/local/include/i386-xbook
	/usr/local/include
	/usr/include/i386-xbook
	/usr/include
## libraries:
	/usr/lib/i386-xbook/
	/usr/lib/
	/lib/i386-xbook/
	/lib/
	/usr/local/lib/i386-xbook
	/usr/local/lib/libc.a (static, libc -> xlibc)
## libtcc1:
	/usr/local/lib/tcc/libtcc1.a
## crt:
	/usr/lib/i386-xbook/crt1.o
	/usr/lib/i386-xbook/crti.o
	/usr/lib/i386-xbook/crtn.o
## elfinterp:
	/lib/ld.so (dynamic)

# Build TCC (I386, Not only xbook)
```Makefile
C_FLAGS = \
	-Wall -O2 -Wdeclaration-after-statement -fno-strict-aliasing \
	-Wno-pointer-sign -Wno-sign-compare -Wno-unused-result -lm -ldl \
	-DCONFIG_TRIPLET="\"i386-xbook\"" -DTCC_TARGET_I386

tcc:
	gcc -o tcc.o -c tcc.c
	gcc -o libtcc.o -c libtcc.c $(C_FLAGS)
	gcc -o tccpp.o -c tccpp.c $(C_FLAGS)
	gcc -o tccgen.o -c tccgen.c $(C_FLAGS)
	gcc -o tccelf.o -c tccelf.c $(C_FLAGS)
	gcc -o tccasm.o -c tccasm.c $(C_FLAGS)
	gcc -o i386-gen.o -c i386-gen.c $(C_FLAGS)
	gcc -o i386-link.o -c i386-link.c $(C_FLAGS)
	gcc -o i386-asm.o -c i386-asm.c $(C_FLAGS)
	ar rcs libtcc.a libtcc.o tccpp.o tccgen.o tccelf.o tccasm.o tccrun.o i386-gen.o i386-link.o i386-asm.o
	gcc -o tcc tcc.o libtcc.a -lm -ldl

libtcc1.a:
	tcc -c libtcc1.c -o libtcc1.o
	tcc -c alloca86.S -o alloca86.o
	tcc -c alloca86-bt.S -o alloca86-bt.o
	tcc -c bcheck.c -o bcheck.o
	# Will not use
	tcc -ar rcs libtcc1.a libtcc1.o alloca86.o alloca86-bt.o bcheck.o
	# Instead of `tcc -ar`
	ar rcs libtcc1.a libtcc1.o alloca86.o alloca86-bt.o bcheck.o
```
# Install TCC (develop/rom/usr/scripts/install-tcc.sh)
```shell
#!/bin/bash
echo "Making c runtime..."
tcc -c /usr/lib/i386-xbook/crt1.S -o /usr/lib/i386-xbook/crt1.o
tcc -c /usr/lib/i386-xbook/crti.S -o /usr/lib/i386-xbook/crti.o
tcc -c /usr/lib/i386-xbook/crtn.S -o /usr/lib/i386-xbook/crtn.o
echo "Making libtcc1.a..."
tcc -c /usr/local/lib/tcc/libtcc1.c -o /usr/local/lib/tcc/libtcc1.o
tcc -c /usr/local/lib/tcc/alloca86.S -o /usr/local/lib/tcc/alloca86.o
tcc -c /usr/local/lib/tcc/alloca86-bt.S -o /usr/local/lib/tcc/alloca86-bt.o
tcc -c /usr/local/lib/tcc/bcheck.c -o /usr/local/lib/tcc/bcheck.o
ar rcs /usr/local/lib/tcc/libtcc1.a /usr/local/lib/tcc/libtcc1.o /usr/local/lib/tcc/alloca86.o /usr/local/lib/tcc/alloca86-bt.o /usr/local/lib/tcc/bcheck.o
```