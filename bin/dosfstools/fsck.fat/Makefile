X_INCDIRS	+= dosfstools/ dosfstools/fsck.fat/
X_LIBS		+= libxlibc.a

NAME		:= fsck.fat
SRC			+= fsck.fat.c

X_CFLAGS	+= -DPACKAGE_NAME=\"dosfstools\" -DPACKAGE_TARNAME=\"dosfstools\" \
			-DPACKAGE_VERSION=\"4.2+git\" -DPACKAGE_STRING=\"dosfstools\ 4.2+git\" \
			-DPACKAGE_BUGREPORT=\"\" -DPACKAGE_URL=\"\" -DPACKAGE=\"dosfstools\" \
			-DVERSION=\"4.2+git\" -DSTDC_HEADERS=1 -DHAVE_SYS_TYPES_H=1 -DHAVE_SYS_STAT_H=1 \
			-DHAVE_STDLIB_H=1 -DHAVE_STRING_H=1 -DHAVE_MEMORY_H=1 -DHAVE_STRINGS_H=1 \
			-DHAVE_INTTYPES_H=1 -DHAVE_STDINT_H=1 -DHAVE_UNISTD_H=1 -D__EXTENSIONS__=1 \
			-D_ALL_SOURCE=1 -D_GNU_SOURCE=1 -D_POSIX_PTHREAD_SEMANTICS=1 -D_TANDEM_SOURCE=1 \
			-DHAVE_ERR_H=1 -DHAVE_LINUX_VERSION_H=1 \
			-DHAVE_SYS_QUEUE_H=1 -DHAVE_ENDIAN_H=1 -DHAVE_VASPRINTF=1 \
			-U__linux__ -DHAVE_DECL_GETMNTINFO=0 -Wall -Wextra -Wno-sign-compare \
			-Wno-missing-field-initializers -Wmissing-prototypes -Wstrict-prototypes \
			-Wwrite-strings -g -O2

define CUSTOM_TARGET_CMD
echo [APP] $@; \
$(LD) $(X_LDFLAGS) $(X_OBJS) -o $@ $(patsubst %, -L%, $(X_LIBDIRS)) --start-group $(patsubst %, -l:%, $(X_LIBS)) --end-group; \
cp $@ $(srctree)/../develop/rom/bin
endef
