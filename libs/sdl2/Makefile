X_INCDIRS	:= xlibc/include \
			pthread/include \
			xtk \
			uview/include \
			dotfont \
			
X_INCDIRS	+= sdl2/SDL2 \
			sdl2/video/yuv2rgb

X_CFLAGS	+= -D__BOOKOS__
SRC	:= \
	src/*.c \
	src/atomic/*.c \
	src/audio/*.c \
	src/cpuinfo/*.c \
	src/events/*.c \
	src/file/*.c \
	src/haptic/*.c \
    src/joystick/*.c \
    src/power/*.c \
    src/render/*.c \
    src/render/software/*.c \
    src/stdlib/*.c \
    src/thread/*.c \
    src/thread/generic/*.c \
	src/timer/*.c \
    src/video/*.c \
    src/video/yuv2rgb/*.c \
    src/audio/disk/*.c \
    src/audio/dummy/*.c \
    src/filesystem/dummy/*.c \
    src/video/dummy/*.c \
    src/haptic/dummy/*.c \
    src/joystick/dummy/*.c \
    src/main/dummy/*.c \
    src/timer/dummy/*.c \
    src/loadso/dummy/*.c \
    src/video/bookos/*.c \
    src/timer/bookos/*.c \
	src/core/linux/SDL_ime.c \
	
TARGET_TYPE	:= static
NAME		:= sdl2