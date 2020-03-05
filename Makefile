CC=gcc
LFLAGS=-lm
CFLAGS=-g -Wall

all:
	$(CC) main.c wave.c containers/*.c parser/*.c math/*.c music/*.c $(CFLAGS) $(LFLAGS)
