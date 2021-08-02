X_LIBS		+= libxlibc.a

NAME		:= tcc
SRC			+= tcc.c
SRC			+= libtcc1.c

X_CFLAGS	+= -Wall -O2 -Wdeclaration-after-statement -fno-strict-aliasing \
			-Wno-pointer-sign -Wno-sign-compare -Wno-unused-result -lm -ldl \
			-DCONFIG_TRIPLET="\"i386-xbook\"" -DTCC_TARGET_I386 -DCONFIG_TCC_STATIC

define CUSTOM_TARGET_CMD
echo [APP] $@; \
$(LD) $(X_LDFLAGS) $(X_OBJS) -o $@ $(patsubst %, -L%, $(X_LIBDIRS)) --start-group $(patsubst %, -l:%, $(X_LIBS)) --end-group; \
cp $@ $(srctree)/../develop/rom/bin
endef
