PROJ=scan
CC=gcc
CFLAGS=-Wall
OBJS=main.o

.PHONY: all
.PHONY: clean

vpath %.c src
vpath %.o bld

bld/%.o: %.c
	$(CC) -c -g $(CFLAGS) -I./inc -o $@ $< 

all: bld/$(PROJ)
bld/$(PROJ): bld/$(OBJS)
	$(CC) $< -o $@

clean:
	rm -f main.o
