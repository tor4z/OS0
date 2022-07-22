CFLAGS += -I./include


.PHONY: all
all: loader.bin boot.bin


boot.o: boot.S
	$(AS) $(ASFLAGS) $< -o $@


loader.o: loader.S
	$(CC) $(CFLAGS) -c $< -o $@


boot.bin: boot.o
	$(LD) $(LDFLAGS) -Ttext $(BOOT_ENTRY) $< -o boot.out
	$(OBJCOPY) -S -O binary boot.out $@


loader.bin: loader.o
	$(LD) $(LDFLAGS) -N -e start -Ttext $(LOADER_ENTRY) $< -o loader.out
	$(OBJCOPY) -S -O binary loader.out $@


.PHONY: clean
clean:
	@-rm -rf *.o *.out *.bin
