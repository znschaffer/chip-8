CC = clang
ROM ?= roms/1-chip8-logo.ch8
DEBUG ?= false
CFLAGS = -DDEBUG=$(DEBUG) -g `pkg-config SDL2 --cflags`
LDFLAGS = -I. `pkg-config SDL2 --libs`
DEPS = core.h

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) 

chip8: main.o core.o
	$(CC) $(LDFLAGS) -o chip8 main.o core.o

clean:
	rm -rf *.o

run:
	./chip8 $(ROM)
