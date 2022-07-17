boot.o: boot.S
	$(AS) $(ASFLAGS) $< -o $@


boot.bin: boot.o
	$(LD) $(LDFLAGS) -Ttext $(BOOT_ENTRY) $< -o boot.out
	$(OBJCOPY) -S -O binary boot.out $@


.PHONY: clean
clean:
	@-rm -rf *.o *.out *.bin
