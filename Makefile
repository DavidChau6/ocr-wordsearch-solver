# OCR Wordsearch Solver

CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g
SDL_CFLAGS = $(shell sdl2-config --cflags)
SDL_LIBS = $(shell sdl2-config --libs) -lSDL2_image
MATH_LIB = -lm

TARGET_GRID = grid_extract
GRID_SRCS = grid_extract.c
GRID_OBJS = $(GRID_SRCS:.c=.o)

all: $(TARGET_GRID)

$(TARGET_GRID): $(GRID_OBJS)
	$(CC) $(CFLAGS) -o $(TARGET_GRID) $(GRID_OBJS) $(SDL_LIBS) $(MATH_LIB)

%.o: %.c
	$(CC) $(CFLAGS) $(SDL_CFLAGS) -c $< -o $@

clean:
	rm -f $(GRID_OBJS) $(TARGET_GRID)

extra-clean:
	rm -f grid_r*.bmp word_*.bmp grid*.bmp words*.bmp

.PHONY: all clean extra-clean
