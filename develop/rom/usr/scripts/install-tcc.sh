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
