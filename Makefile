CC := gcc
AS := as
LD := ld
MAKE := make
OBJCOPY := objcopy
QEMU := qemu-system-i386
BOCHS := bochs

BOOT_ENTRY := 0x7c00
LOADER_ENTRY := 0x9200
CFLAGS := -Wall -nostdinc -fno-builtin -m32 -MD
CFLAGS += -Wno-format -Wno-unused -Werror
CFLAGS += -fno-omit-frame-pointer
ASFLAGS := --32
LDFLAGS := -m elf_i386

BOOT_DIR := boot
DOC_DIR := Documentation

# export variable into sub-makefile
export


.PHONY: all
all: doc_html


.PHONY: boot.bin
boot.bin:
	$(MAKE) -C $(BOOT_DIR) boot.bin


.PHONY: loader.bin
loader.bin:
	$(MAKE) -C $(BOOT_DIR) loader.bin


a.img: boot.bin
	dd if=boot/boot.bin of=a.img bs=512 count=1
	dd if=/dev/zero of=a.img bs=512 count=2879 skip=1 seek=1


.PHONY: copy_loader
copy_loader: a.img loader.bin
	mkdir -p /tmp/floppy
	-sudo umount /tmp/floppy
	sudo mount -o loop a.img /tmp/floppy
	sudo cp boot/loader.bin /tmp/floppy
	sudo umount /tmp/floppy


.PHONY: build
build: copy_loader


.PHONY: rebuild
rebuild: clean build


.PHONY: qemu
qemu: build
	$(QEMU) -drive file=a.img,if=floppy


.PHONY: bochs
bochs: build bochsrc.yaml
	$(BOCHS) -f bochsrc.yaml


.PHONY: rebuild_qemu
rebuild_qemu: clean qemu


.PHONY: rebuild_bochs
rebuild_bochs: clean bochs


.PHONY: doc_html
doc_html:
	$(MAKE) -C $(DOC_DIR) html


.PHONY: clean
clean:
	@-rm -f *.img
	$(MAKE) -C $(BOOT_DIR) clean
	$(MAKE) -C $(DOC_DIR) clean
