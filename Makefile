CC=gcc
CFLAGS = -Wall -c -m64 
LFLAGS = -Wall -m64

sha:	sha.o
	$(CC) $(LFLAGS) sha.o -o sha

sha.o:	sha.c sha.h
	$(CC) $(CFLAGS) sha.c
	
