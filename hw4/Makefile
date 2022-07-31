CC=gcc
CFLAGS= -Wall
EXTRA_CFLAGS= 
 
%.o: %.c
    gcc -c -o $@ $< $(CFLAGS) $(EXTRA_CFLAGS)
 
all: forkpipe nsd
 
forkpipe: forkpipe.o
    gcc -o forkpipe forkpipe.c $(CFLAGS)
 
nsd: nd.o nsd_main.o nsd.o nd.h nsd.h
    gcc -o nsd nd.c nsd_main.c nsd.c $(CFLAGS)
 
zip:
    tar czf osy04.tgz Makefile *.[ch]
clean:
    rm -f *.o
    rm -f *.d
    rm nsd forkpipe
    rm osy04.tgz