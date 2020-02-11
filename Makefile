CC=gcc
LFLAGS=-lm
CFLAGS=-g -Wall

all:
	$(CC) main.c music.c wave.c $(CFLAGS) $(LFLAGS)
