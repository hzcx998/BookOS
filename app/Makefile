sinclude ../scripts/env.mk
sinclude ../scripts/localenv.mk

LIBS_DIR	:= ../libs

X_ASFLAGS	:= $(ENV_AFLAGS)
X_CFLAGS	:= $(ENV_CFLAGS)

X_INCDIRS	:= $(LIBS_DIR)/xlibc/include $(LIBS_DIR)/pthread/include $(LIBS_DIR)/netsocket/include \
				$(LIBS_DIR)/xlibcpp/include \
				$(LIBS_DIR)/zlib \
				$(LIBS_DIR)/libpng \
				$(LIBS_DIR)/jpeg \
				$(LIBS_DIR)/lvgl \
				$(LIBS_DIR)/lvgl/include \
				$(LIBS_DIR)/uview/include \
				$(LIBS_DIR)/dotfont \
				$(LIBS_DIR)/xtk \
				$(LIBS_DIR)/gato \
				$(LIBS_DIR)/freetype/include \
				$(LIBS_DIR)/cairo \
				$(LIBS_DIR)/pixman \
				$(LIBS_DIR)/sdl2/SDL2 \
				$(LIBS_DIR)/sdl2 \
				$(LIBS_DIR)/lua \
				$(LIBS_DIR)/libwebp/src \
				$(LIBS_DIR)/libtiff \
				$(LIBS_DIR)/SDL2_image \
				$(LIBS_DIR)/SDL2_ttf \
				$(LIBS_DIR)/iniparser \
				$(LIBS_DIR)/cjson \
					
X_LIBDIRS	+= $(LIBS_DIR)/xlibc $(LIBS_DIR)/pthread \
				$(LIBS_DIR)/netsocket \
				$(LIBS_DIR)/zlib \
				$(LIBS_DIR)/libpng \
				$(LIBS_DIR)/jpeg \
				$(LIBS_DIR)/lvgl \
				$(LIBS_DIR)/uview \
				$(LIBS_DIR)/xtk \
				$(LIBS_DIR)/gato \
				$(LIBS_DIR)/freetype \
				$(LIBS_DIR)/cairo \
				$(LIBS_DIR)/pixman \
				$(LIBS_DIR)/sdl2 \
				$(LIBS_DIR)/lua \
				$(LIBS_DIR)/libwebp \
				$(LIBS_DIR)/libtiff \
				$(LIBS_DIR)/SDL2_image \
				$(LIBS_DIR)/SDL2_ttf \
				$(LIBS_DIR)/iniparser \
				$(LIBS_DIR)/cjson \
				
X_LDFLAGS	:= $(ENV_LDFLAGS) $(ENV_USER_LD_SCRIPT)

LD			:=  $(ENV_LD)
AS			:= $(ENV_AS)

export AS LD

# bin
MODULE      +=  winlogin
MODULE      +=  freeterm
MODULE      +=  terminal
MODULE      +=  desktop
MODULE      +=  taskbar
MODULE      +=  gato
MODULE      +=  infones
MODULE      +=  invader
MODULE      +=  xtk
MODULE      +=  sdl2
MODULE      +=  lite
#MODULE      +=  showfont
#MODULE      +=  showimage
