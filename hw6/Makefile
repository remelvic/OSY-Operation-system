CFLAGS= -Wall -g -O2 -pthread
EXTRA_CFLAGS= #-fsanitize=address -fno-omit-frame-pointer
%.o: %.cpp
	g++ -cpp -o $@ $< $(CFLAGS) $(EXTRA_CFLAGS)
all: factory
 
factory: factory.o
	g++ -o factory factory.cpp $(CFLAGS) $(EXTRA_CFLAGS)
clean:
	rm -f *.o
	rm factory
	rm osy06.tgz
zip:
	tar czf osy06.tgz Makefile *.[cpph]
