CFLAGS= -Wall -g -O2 -pthread
EXTRA_CFLAGS= -fsanitize=address -fno-omit-frame-pointer
%.o: %.c
	gcc -c -o $@ $< $(CFLAGS) $(EXTRA_CFLAGS)
 
all: prod-cons 
 
prod-cons: prod-cons.o
	gcc -o prod-cons prod-cons.c $(CFLAGS) $(EXTRA_CFLAGS)
clean:
	rm -f *.o
	rm prod-cons
	rm osy05.tgz
zip:
	tar czf osy05.tgz Makefile *.[ch]
