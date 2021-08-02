sinclude ../scripts/localenv.mk
sinclude ../scripts/env.mk

LIBS_DIR	:= ../libs

X_ASFLAGS	:= $(ENV_AFLAGS)
X_CFLAGS	:= $(ENV_CFLAGS)

X_INCDIRS	:= 	$(LIBS_DIR)/xlibc/include $(LIBS_DIR)/jpeg $(LIBS_DIR)/libpng $(LIBS_DIR)/pthread/include \
				$(LIBS_DIR)/zlib $(LIBS_DIR)/iniparser $(LIBS_DIR)/netsocket/include

X_LDFLAGS	:=  $(ENV_LDFLAGS) $(ENV_USER_LD_SCRIPT)
X_LIBDIRS	:= 	$(LIBS_DIR)/xlibc $(LIBS_DIR)/jpeg $(LIBS_DIR)/libpng $(LIBS_DIR)/pthread $(LIBS_DIR)/zlib \
				$(LIBS_DIR)/iniparser $(LIBS_DIR)/netsocket \
				$(LIBS_DIR)/xcrt \

X_LIBS		:= libxcrt.a

LD			:=  $(ENV_LD)
AS			:=	$(ENV_AS)

export AS LD

MODULE      +=  init
MODULE      +=  login
MODULE      +=  userctl
MODULE      +=  service
MODULE      +=  testserv
MODULE      +=  netserv