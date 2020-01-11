CC=gcc
LFLAGS=-lasound -lm
CFLAGS=-g -Wall

all:
	$(CC) main.c sound_engine.c music.c wave.c $(CFLAGS) $(LFLAGS)
