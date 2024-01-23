DEFAULT := build
CC = gcc
CFLAGS = -Wpedantic -Wall -Wextra -Werror
SRCS = main.c request.c server.c
SHELL = bash

all: build
build: $(SRCS)
	$(CC) $(CFLAGS) main.c request.c server.c -o serve

.PHONY: clean
clean:
	rm -rf *.o serve
