
X_INCDIRS	+= infones/core
X_LIBS		+= libxlibc.a libxtk.a libuview.a libjpeg.a libzlib.a libpng.a

X_CFLAGS    := -fno-builtin -fno-PIE -m32 -fno-stack-protector -std=gnu99 -O3

NAME		:= infones
SRC			+= infoNES_System_Book.c

define CUSTOM_TARGET_CMD
echo [APP] $@; \
$(LD) $(X_LDFLAGS) $(X_OBJS) -o $@ $(patsubst %, -L%, $(X_LIBDIRS)) --start-group $(patsubst %, -l:%, $(X_LIBS)) --end-group; \
cp $@ $(srctree)/../develop/rom/usr/local/infones
endef
