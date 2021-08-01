# The tools of TCC: ar
```Makefile
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
