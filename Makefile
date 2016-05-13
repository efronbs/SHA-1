CC=gcc
CFLAGS = -Wall -c -O3 -m64 
LFLAGS = -Wall -O3 -m64

sha:	sha.o setup.o util.o hash_gen.o
	$(CC) $(LFLAGS) hash_gen.o util.o setup.o sha.o -o sha

sha.o:	sha.c sha.h
	$(CC) $(CFLAGS) sha.c

setup.o: setup.c sha.h
	$(CC) $(CFLAGS) setup.c

util.o: util.c sha.h
	$(CC) $(CFLAGS) util.c	

hash_gen.o: hash_gen.c sha.h
	$(CC) $(CFLAGS) hash_gen.c
