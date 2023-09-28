PROGRAM = chip8
CC=clang
LIBS = `pkg-config SDL2 --libs`
CFLAGS= -g -Wall -Wextra `pkg-config SDL2 --cflags`

default: $(PROGRAM)
all: default

OBJECTS = $(patsubst %.c, %.o, $(wildcard *.c))
HEADERS = $(wildcard *.h)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -Wall $(LIBS) -o $@


