X_LIBS		+= libxlibc.a libuview.a libzlib.a libpng.a libjpeg.a libxtk.a \
				libiniparser.a libcjson.a

NAME		:= desktop
SRC			+= *.c

define CUSTOM_TARGET_CMD
echo [APP] $@; \
$(LD) $(X_LDFLAGS) $(X_OBJS) -o $@ $(patsubst %, -L%, $(X_LIBDIRS)) --start-group $(patsubst %, -l:%, $(X_LIBS)) --end-group; \
cp $@ $(srctree)/../develop/rom/system/programs
endef
