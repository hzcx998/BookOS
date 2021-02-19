X_INCDIRS	:= xlibc/include \
			xtk \
			uview/include \
			dotfont \
			sdl2/SDL2 \
			libwebp/src \
			libtiff \

X_INCDIRS	+= jpeg \
			libzlib \
			libpng \

X_CFLAGS	+= -DSDL_IMAGE_USE_COMMON_BACKEND \
			-DLOAD_BMP -DLOAD_GIF -DLOAD_LBM -DLOAD_PCX -DLOAD_PNM \
			-DLOAD_SVG -DLOAD_TGA -DLOAD_XCF -DLOAD_XPM -DLOAD_XV \
			-DLOAD_JPG -DLOAD_PNG -DLOAD_WEBP

SRC			:= *.c

TARGET_TYPE	:= static
NAME		:= SDL2_image
