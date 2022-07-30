S_SRCS  := $(wildcard ./*.S)
C_SRCS  := $(wildcard ./*.c)
OBJS    := $(patsubst %.S,%.o,$(S_SRCS))
OBJS    += $(patsubst %.c,%.o,$(C_SRCS))


%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@


%.o: %.S
	$(CC) $(CFLAGS) -c $< -o $@


kernel.bin: $(OBJS)
	$(LD) $(LDFLAGS) -s -Ttext $(KERNEL_ENTRY) $^ -o $@


.PHONY: clean
clean:
	@-rm -f *.o *.bin
