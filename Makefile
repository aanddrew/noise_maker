CC=gcc
LFLAGS=-lm
CFLAGS=-g -Wall -Wextra -Wunused -Wunreachable-code

all:
	$(CC) main.c wave.c containers/*.c parser/*.c math/*.c music/*.c $(CFLAGS) $(LFLAGS) -o noise_maker
