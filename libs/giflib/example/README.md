# Example of giflib

```bash
	C_FLAGS = -std=gnu99 -fPIC -Wall -Wno-format-truncation -O2

libutil.a:
	gcc $(C_FLAGS) -c -o qprintf.o qprintf.c
	gcc $(C_FLAGS) -c -o quantize.o quantize.c
	gcc $(C_FLAGS) -c -o getarg.o getarg.c
	ar rcs libutil.a qprintf.o quantize.o getarg.o

example:
	gcc $(C_FLAGS) gif2rgb.c libgif.a libutil.a -lm -o gif2rgb
	gcc $(C_FLAGS) gifbuild.c libgif.a libutil.a -lm -o gifbuild
	gcc $(C_FLAGS) giffix.c libgif.a libutil.a -lm -o giffix
	gcc $(C_FLAGS) giftext.c libgif.a libutil.a -lm -o giftext
	gcc $(C_FLAGS) giftool.c libgif.a libutil.a -lm -o giftool
	gcc $(C_FLAGS) gifclrmp.c libgif.a libutil.a -lm -o gifclrmp
	gcc $(C_FLAGS) gifbg.c libgif.a libutil.a -lm -o gifbg
	gcc $(C_FLAGS) gifcolor.c libgif.a libutil.a -lm -o gifcolor
	gcc $(C_FLAGS) gifecho.c libgif.a libutil.a -lm -o gifecho
	gcc $(C_FLAGS) giffilter.c libgif.a libutil.a -lm -o giffilter
	gcc $(C_FLAGS) gifhisto.c libgif.a libutil.a -lm -o gifhisto
	gcc $(C_FLAGS) gifinto.c libgif.a libutil.a -lm -o gifinto
	gcc $(C_FLAGS) gifsponge.c libgif.a libutil.a -lm -o gifsponge
	gcc $(C_FLAGS) gifwedge.c libgif.a libutil.a -lm -o gifwedge
```