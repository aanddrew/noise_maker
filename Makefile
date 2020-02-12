CC=gcc
LFLAGS=-lm
CFLAGS=-g -Wall

all:
	$(CC) main.c music.c wave.c parser.c math_parser.c $(CFLAGS) $(LFLAGS)
