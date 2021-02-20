X_LIBS		+= libxlibc.a libsdl2.a libpthread.a \
			libzlib.a libpng.a libjpeg.a libuview.a libxtk.a \
			liblua.a libfreetype.a libSDL2_ttf.a

X_CFLAGS	:= -fno-strict-aliasing -fno-builtin -Wall -Wunused \
			-fno-PIE -m32 -fno-stack-protector -std=c++11 -O3 -D__BOOKOS

NAME		:= SDL2snake
SRC			:= src/*.cpp

define CUSTOM_TARGET_CMD
echo [APP] $@; \
$(LD) $(X_LDFLAGS) $(X_OBJS) -o $@ $(patsubst %, -L%, $(X_LIBDIRS)) --start-group $(patsubst %, -l:%, $(X_LIBS)) --end-group; \
cp $@ $(srctree)/../develop/rom/app
endef
