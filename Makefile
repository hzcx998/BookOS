# MIT License
# Copyright (c) 2020 Jason Hu, Zhu Yu
all:

# tools
MAKE		= make
TOOL_DIR	= tools
FATFS_DIR	= $(TOOL_DIR)/fatfs
GRUB_DIR	= $(TOOL_DIR)/grub-2.04
BIOS_FW_DIR	= $(TOOL_DIR)/bios_fw

TRUNC		= truncate
RM			= rm
DD			= dd
MKDIR		= mkdir
OBJDUMP		= objdump
CP			= cp
MKFS		= mkfs.msdos
MCOPY		= mtools -c mcopy

# virtual machine
QEMU 		= qemu-system-i386

# images and rom
IMAGE_DIR	= develop/image
FLOPPYA_IMG	= $(IMAGE_DIR)/a.img
HDA_IMG		= $(IMAGE_DIR)/c.img
HDB_IMG		= $(IMAGE_DIR)/d.img
ROM_DIR		= develop/rom

BOOT_DISK	= $(FLOPPYA_IMG)
FS_DISK		= $(HDB_IMG)

# image size
FLOPPYA_SZ	= 1474560  # 1.44 MB
HDA_SZ		= 33554432 # 64 MB: 33554432
HDB_SZ		= 67108864

# environment dir

LIBS_DIR	= ./libs
SBIN_DIR	= ./sbin
BIN_DIR		= ./bin
APP_DIR		= ./app

#kernel disk
LOADER_OFF 	= 2
LOADER_CNTS = 8

SETUP_OFF 	= 10
SETUP_CNTS 	= 90

KERNEL_OFF 	= 100
KERNEL_CNTS	= 1024		# assume 512kb 

# kernel dir
KERNSRC		= kernel

# OS Name
OS_NAME = XBook

# kernel boot binary
BOOT_BIN 	= $(KERNSRC)/boot/boot.bin
LOADER_BIN 	= $(KERNSRC)/boot/loader.bin
SETUP_BIN 	= $(KERNSRC)/boot/setup.bin
# kernel file
KERNEL_ELF 	= $(KERNSRC)/kernel.elf
# kernel file
KERNEL_ISO 	= $(KERNSRC)/$(OS_NAME).iso

REMOTE_KERNEL_DIR 	= ../xbook2/src

REMOTE_KERNEL_ELF 	= $(REMOTE_KERNEL_DIR)/kernel.elf
REMOTE_BOOT_BIN 	= $(REMOTE_KERNEL_DIR)/arch/x86/boot/myboot/boot.bin
REMOTE_LOADER_BIN 	= $(REMOTE_KERNEL_DIR)/arch/x86/boot/myboot/loader.bin
REMOTE_SETUP_BIN 	= $(REMOTE_KERNEL_DIR)/arch/x86/boot/myboot/setup.bin

REMOTE_KERNEL_ISO 	= $(REMOTE_KERNEL_DIR)/$(OS_NAME).iso

# boot mode
export BOOT_GRUB2_MODE = GRUB2
export BOOT_LEGACY_MODE = LEGACY

# set default boot mode
export BOOT_MODE ?= $(BOOT_GRUB2_MODE)

# is efi mode? (y/n)
EFI_BOOT_MODE ?= n
# is qemu fat fs? (y/n)
QEMU_FAT_FS ?= n

# is livecd mode? (y/n)
KERN_LIVECD_MODE ?= n

# 参数
.PHONY: all clean wrdisk build debuild sync

all: 
	$(MAKE) -s -C  $(LIBS_DIR) && \
	$(MAKE) -s -C  $(SBIN_DIR) && \
	$(MAKE) -s -C  $(BIN_DIR) && \
	$(MAKE) -s -C  $(APP_DIR)

clean: 
	$(MAKE) -s -C  $(LIBS_DIR) clean && \
	$(MAKE) -s -C  $(SBIN_DIR) clean && \
	$(MAKE) -s -C  $(BIN_DIR) clean && \
	$(MAKE) -s -C  $(APP_DIR) clean

wrdisk: all
ifeq ($(BOOT_MODE),$(BOOT_LEGACY_MODE))
	$(DD) if=$(BOOT_BIN) of=$(BOOT_DISK) bs=512 count=1 conv=notrunc
	$(DD) if=$(LOADER_BIN) of=$(BOOT_DISK) bs=512 seek=$(LOADER_OFF) count=$(LOADER_CNTS) conv=notrunc
	$(DD) if=$(SETUP_BIN) of=$(BOOT_DISK) bs=512 seek=$(SETUP_OFF) count=$(SETUP_CNTS) conv=notrunc
	$(DD) if=$(KERNEL_ELF) of=$(BOOT_DISK) bs=512 seek=$(KERNEL_OFF) count=$(KERNEL_CNTS) conv=notrunc
else
ifeq ($(BOOT_MODE),$(BOOT_GRUB2_MODE))
	@$(MAKE) -s -C $(GRUB_DIR) KERNEL=$(subst $(KERNSRC)/,,$(KERNEL_ELF)) OS_NAME=$(OS_NAME)
endif
endif
ifeq ($(QEMU_FAT_FS),n)
	$(MKFS) -F 32 $(FS_DISK)
	$(MCOPY) -i $(FS_DISK) -/ $(ROM_DIR)/* ::./
endif

# 构建环境。镜像>工具>环境>rom
build: 
	-$(MKDIR) $(IMAGE_DIR)
	-$(MKDIR) $(ROM_DIR)/bin
	-$(MKDIR) $(ROM_DIR)/sbin
	-$(MKDIR) $(ROM_DIR)/app
	-$(MKDIR) $(ROM_DIR)/system/programs
	$(TRUNC) -s $(FLOPPYA_SZ) $(FLOPPYA_IMG)
	$(TRUNC) -s $(HDA_SZ) $(HDA_IMG)
	$(TRUNC) -s $(HDB_SZ) $(HDB_IMG) 
ifeq ($(OS),Windows_NT)
else
	$(MAKE) -s -C  $(FATFS_DIR)
endif
	$(MAKE) -s -C  $(LIBS_DIR)
	$(MAKE) -s -C  $(SBIN_DIR)
	$(MAKE) -s -C  $(BIN_DIR)
	$(MAKE) -s -C  $(APP_DIR)
ifeq ($(QEMU_FAT_FS),n)
	$(MKFS) -F 32 $(FS_DISK)
	$(MCOPY) -i $(FS_DISK) -/ $(ROM_DIR)/* ::./
endif

# 清理环境。
debuild: 
ifeq ($(OS),Windows_NT)
else
	$(MAKE) -s -C  $(FATFS_DIR) clean
endif
	$(MAKE) -s -C  $(LIBS_DIR) clean
	$(MAKE) -s -C  $(SBIN_DIR) clean
	$(MAKE) -s -C  $(BIN_DIR) clean
	$(MAKE) -s -C  $(APP_DIR) clean
	-$(RM) -r $(ROM_DIR)/bin
	-$(RM) -r $(ROM_DIR)/sbin
	-$(RM) -r $(ROM_DIR)/app
	-$(RM) -r $(ROM_DIR)/acct
	-$(RM) -r $(ROM_DIR)/system/programs
	-$(RM) -r $(IMAGE_DIR)

#-hda $(HDA_IMG) -hdb $(HDB_IMG)
# 网卡配置: 
#	-net nic,vlan=0,model=rtl8139,macaddr=12:34:56:78:9a:be
# 网络模式：
#	1.User mode network(Slirp) :User网络
#		-net user
#	2.Tap/tun network : Tap网络
#		-net tap
# 		-net tap,vlan=0,ifname=tap0
#	example: -net nic,model=rtl8139 -net tap,ifname=tap0,script=no,downscript=no 
		
# 音频配置：
# 	a.使用蜂鸣器：-soundhw pcspk
#	b.使用声霸卡：-soundhw sb16
# 控制台串口调试： -serial stdio 

# 磁盘配置：
#	1. IDE DISK：-hda $(HDA_IMG) -hdb $(HDB_IMG) \
# 	2. AHCI DISK: -drive id=disk0,file=$(HDA_IMG),if=none \
		-drive id=disk1,file=$(HDB_IMG),if=none \
		-device ahci,id=ahci \
		-device ide-drive,drive=disk0,bus=ahci.0 \
		-device ide-drive,drive=disk1,bus=ahci.1 \

# 时间配置：使用主机一样的时间
# 		-rtc base=localtime	

ifeq ($(OS),Windows_NT)
QEMU_KVM := -accel hax
else
QEMU_KVM := -enable-kvm
endif
QEMU_KVM := # no virutal

ifeq ($(QEMU_FAT_FS),y)
	HDB_IMG :=fat:rw:./develop/rom
endif

QEMU_ARGUMENT := -m 1024M $(QEMU_KVM) \
		-name "BookOS Development Platform for x86" \
		-rtc base=localtime \
		-boot a \
		-serial stdio \

ifeq ($(KERN_LIVECD_MODE),n)
QEMU_ARGUMENT += -drive id=disk0,file=$(HDA_IMG),format=raw,if=none \
		-drive id=disk1,file=$(HDB_IMG),format=raw,if=none \
		-device ahci,id=ahci \
		-device ide-hd,drive=disk0,bus=ahci.0 \
		-device ide-hd,drive=disk1,bus=ahci.1
endif

ifeq ($(BOOT_MODE),$(BOOT_LEGACY_MODE))
QEMU_ARGUMENT += -drive file=$(FLOPPYA_IMG),format=raw,index=0,if=floppy
endif

#		-fda $(FLOPPYA_IMG) -hda $(HDA_IMG) -hdb $(HDB_IMG) -boot a \
#		-net nic,model=rtl8139 -net tap,ifname=tap0,script=no,downscript=no 

# qemu启动
run: wrdisk
ifeq ($(BOOT_MODE),$(BOOT_LEGACY_MODE))
	$(QEMU) $(QEMU_ARGUMENT)
else
ifeq ($(BOOT_MODE),$(BOOT_GRUB2_MODE))
ifeq ($(EFI_BOOT_MODE),n)
	$(QEMU) $(QEMU_ARGUMENT) -cdrom $(KERNEL_ISO)
else
	$(QEMU) $(QEMU_ARGUMENT) -bios $(BIOS_FW_DIR)/IA32_OVMF.fd -cdrom $(KERNEL_ISO)
endif
endif
endif

sync:
ifeq ($(BOOT_MODE),$(BOOT_LEGACY_MODE))
	$(CP) $(REMOTE_KERNEL_ELF) $(KERNEL_ELF)
	$(CP) $(REMOTE_BOOT_BIN) $(BOOT_BIN)
	$(CP) $(REMOTE_LOADER_BIN) $(LOADER_BIN)
	$(CP) $(REMOTE_SETUP_BIN) $(SETUP_BIN)
else
	$(CP) $(REMOTE_KERNEL_ELF) $(KERNEL_ELF)
	$(CP) $(REMOTE_KERNEL_ISO) $(KERNEL_ISO)
endif