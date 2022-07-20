.PHONY: all
all: loader.bin boot.bin


boot.o: boot.S
	$(AS) $(ASFLAGS) $< -o $@


loader.o: loader.S
	$(AS) $(ASFLAGS) $< -o $@


boot.bin: boot.o
	$(LD) $(LDFLAGS) -Ttext $(BOOT_ENTRY) $< -o boot.out
	$(OBJCOPY) -S -O binary boot.out $@


loader.bin: loader.o
	$(LD) $(LDFLAGS) $< -o loader.out
	$(OBJCOPY) -S -O binary loader.out $@


.PHONY: clean
clean:
	@-rm -rf *.o *.out *.bin
