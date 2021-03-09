X_LIBS		+= libxlibc.a libxtk.a libuview.a libzlib.a libpng.a libjpeg.a libsdl2.a libSDL2_ttf.a \
				libpthread.a libfreetype.a 

X_INCDIRS	+= terminal/vt100

NAME		:= terminal
SRC			+= main.c vt100/vt100.c

define CUSTOM_TARGET_CMD
echo [APP] $@; \
$(LD) $(X_LDFLAGS) $(X_OBJS) -o $@ $(patsubst %, -L%, $(X_LIBDIRS)) --start-group $(patsubst %, -l:%, $(X_LIBS)) --end-group; \
cp $@ $(srctree)/../develop/rom/app
endef
