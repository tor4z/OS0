kernel.o: kernel.S
	$(CC) $(CFLAGS) -c $< -o $@


kernel.bin: kernel.o
	$(LD) $(LDFLAGS) -s -Ttext $(KERNEL_ENTRY) $< -o $@


.PHONY: clean
clean:
	@-rm -f *.o *.bin
