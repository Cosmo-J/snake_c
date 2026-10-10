CC = gcc

CFLAGS = -Wall -Wextra -std=c11 $(shell pkg-config --cflags sdl3 sdl3-image)
LDLIBS = $(shell pkg-config --libs sdl3 sdl3-image)

SRC = src/*.c
HDR = src/*.h
TARGET = build

$(TARGET): $(SRC) $(HDR)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) $(LDLIBS) -MMD -MP

clean:
	rm -f $(TARGET)
