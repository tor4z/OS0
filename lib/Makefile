C_SRCS := $(wildcard *.c)
OBJS   := $(patsubst %.c,%.o,$(C_SRCS))

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@


$(KLIB): $(OBJS)
	$(AR) rcs $@ $^


.PHONY: clean
clean:
	@-rm -f *.o *.a *.d
