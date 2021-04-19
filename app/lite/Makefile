X_LIBS		+= libxlibc.a libsdl2.a libpthread.a \
			libzlib.a libpng.a libjpeg.a libuview.a libxtk.a \
			liblua.a 

X_CFLAGS	+= -D__BOOKOS__

X_INCDIRS	+= lite

NAME		:= lite
SRC			:= *.c
SRC			+= lib/stb/*.c
#SRC		+= lib/lua54/*.c
SRC			+= api/*.c

define CUSTOM_TARGET_CMD
echo [APP] $@; \
$(LD) $(X_LDFLAGS) $(X_OBJS) -o $@ $(patsubst %, -L%, $(X_LIBDIRS)) --start-group $(patsubst %, -l:%, $(X_LIBS)) --end-group; \
cp $@ $(srctree)/../develop/rom/usr/local/lite
endef
