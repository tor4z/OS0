CC := gcc
AS := as
LD := ld
AR := ar
MAKE := make
OBJCOPY := objcopy
QEMU := qemu-system-i386
BOCHS := bochs
ROOT_PATH := $(shell pwd)


BOOT_DIR := boot
KLIB_DIR := lib
KERNEL_DIR := kernel
DOC_DIR := Documentation

KLIB := libk.a
BOOT := boot.bin
LOADER := loader.bin
KERNEL := kernel.bin
IMG := a.img
BOCHS_CONF := bochsrc.yaml

KLIB_PATH := $(ROOT_PATH)/$(KLIB_DIR)/$(KLIB)
BOOT_PATH := $(ROOT_PATH)/$(BOOT_DIR)/$(BOOT)
LOADER_PATH := $(ROOT_PATH)/$(BOOT_DIR)/$(LOADER)
KERNEL_PATH := $(ROOT_PATH)/$(KERNEL_DIR)/$(KERNEL)
KLIB_SEARCH_PATH := $(ROOT_PATH)/$(KLIB_DIR)


BOOT_ENTRY := 0x7c00
LOADER_ENTRY := 0x9200
KERNEL_ENTRY := 0x30400
CFLAGS := -Wall -nostdinc -fno-builtin -m32 -MD
CFLAGS += -Wno-format -Wno-unused -Werror
CFLAGS += -fno-omit-frame-pointer
CFLAGS += -I$(ROOT_PATH)/include
ASFLAGS := --32
LDFLAGS := -m elf_i386


# export variable into sub-makefile
export


.PHONY: all
all: doc_html $(BOOT) $(LOADER) $(KERNEL)


.PHONY: $(BOOT)
$(BOOT):
	$(MAKE) -C $(BOOT_DIR) $@


.PHONY: $(LOADER)
$(LOADER):
	$(MAKE) -C $(BOOT_DIR) $@


.PHONY: $(KLIB)
$(KLIB):
	$(MAKE) -C $(KLIB_DIR) $@


.PHONY: $(KERNEL)
$(KERNEL): $(KLIB)
	$(MAKE) -C $(KERNEL_DIR) $@


$(IMG): $(BOOT)
	dd if=$(BOOT_PATH) of=$@ bs=512 count=1
	dd if=/dev/zero of=$@ bs=512 count=2879 skip=1 seek=1


.PHONY: copy_loader
copy_loader: $(IMG) $(LOADER) $(KERNEL)
	mkdir -p /tmp/floppy
	-sudo umount /tmp/floppy
	sudo mount -o loop $(IMG) /tmp/floppy
	sudo cp $(LOADER_PATH) /tmp/floppy
	sudo cp $(KERNEL_PATH) /tmp/floppy
	sudo umount /tmp/floppy


.PHONY: build
build: copy_loader


.PHONY: rebuild
rebuild: clean build


.PHONY: qemu
qemu: build
	$(QEMU) -drive file=$(IMG),if=floppy


.PHONY: bochs
bochs: build $(BOCHS_CONF)
	$(BOCHS) -q -f $(BOCHS_CONF)


.PHONY: rebuild_qemu
rebuild_qemu: clean qemu


.PHONY: rebuild_bochs
rebuild_bochs: clean bochs


.PHONY: doc_html
doc_html:
	$(MAKE) -C $(DOC_DIR) html


.PHONY: clean
clean:
	@-rm -f *.img *.log
	$(MAKE) -C $(BOOT_DIR) clean
	$(MAKE) -C $(DOC_DIR) clean
	$(MAKE) -C $(KERNEL_DIR) clean
