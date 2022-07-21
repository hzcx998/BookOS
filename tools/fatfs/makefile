# 工具
RM			= rm

# 系统环境变量
ifeq ($(OS),Windows_NT)
	# !!! if you wanto complie this tools in windows, you need use gcc for windows
	# and config your own path. Below is my path.
	CC       = 	D:/Softwares/Mingw/bin/gcc.exe 		# need mingw gcc for windows
	# need mingw include and libs
	INCS     = 	-I"D:/Softwares/Mingw/include" \
				-DWIN32

	LIBS 	= 	-L"D:/Softwares/Mingw/lib" \
				-static-libgcc
	
	# out file
	BIN = ./fatfs.exe

else

	ifeq ($(shell uname),Linux)
		CC       	= gcc
		INCS     = -I /usr/include -I /usr/local/include
		LIBS     = -static-libgcc
		# out file
		BIN = ./fatfs
	endif

	ifeq ($(shell uname),Darwin)
		CC       	= gcc
		INCS     = -I /usr/include -I /usr/local/include
		LIBS     = 
		# out file
		BIN = ./fatfs

	endif
endif

# 标志
CFLAGS   = $(INCS) -g3

# 对象		
OBJS = 	main.o \
		driver.o \
        diskio.o \
        ff.o \
        ffsystem.o \
        ffunicode.o \

.PHONY: all clean

# 所有
all: $(BIN) 

$(BIN): $(OBJS)
	$(CC) $(OBJS) -o $(BIN) $(LIBS)

%.o : %.c
	@echo [CC] $@ $<
	@$(CC) -c $(CFLAGS) -o $@ $<

# 清理文件
clean: 
	-$(RM) $(BIN)
	-$(RM) $(OBJS)
	
