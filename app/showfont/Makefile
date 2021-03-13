X_LIBS		+= libxlibc.a libsdl2.a libpthread.a \
			libzlib.a libpng.a libjpeg.a libuview.a libxtk.a \
			
X_LIBS		+= libfreetype.a libSDL2_ttf.a			 

NAME		:= showfont
SRC			+= main.c

define CUSTOM_TARGET_CMD
echo [APP] $@; \
$(LD) $(X_LDFLAGS) $(X_OBJS) -o $@ $(patsubst %, -L%, $(X_LIBDIRS)) --start-group $(patsubst %, -l:%, $(X_LIBS)) --end-group; \
cp $@ $(srctree)/../develop/rom/usr/local
endef
