CC := gcc
AS := as
LD := ld
MAKE := make
OBJCOPY := objcopy
QEMU := qemu-system-i386
BOCHS := bochs

BOOT_ENTRY := 0x7c00
CFLAGS := -Wall
ASFLAGS := --32
LDFLAGS := -m elf_i386

BOOT_DIR := boot
DOC_DIR := Documentation

export

.PHONY: all
all: doc_html


.PHONY: boot.bin
boot.bin:
	$(MAKE) -C $(BOOT_DIR) boot.bin


a.img: boot.bin
	dd if=boot/boot.bin of=a.img bs=512 count=1
	dd if=/dev/zero of=a.img bs=512 count=2879 skip=1 seek=1


.PHONY: qemu
qemu: a.img
	$(QEMU) -drive file=a.img,if=floppy


.PHONY: bochs
bochs: a.img bochsrc
	$(BOCHS) -f bochsrc


.PHONY: doc_html
doc_html:
	$(MAKE) -C $(DOC_DIR) html


.PHONY: clean
clean:
	@-rm -f *.img
	$(MAKE) -C $(BOOT_DIR) clean
	$(MAKE) -C $(DOC_DIR) clean
