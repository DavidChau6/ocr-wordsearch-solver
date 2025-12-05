# OCR Wordsearch Solver

CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g
SDL_CFLAGS = $(shell sdl2-config --cflags)
SDL_LIBS = $(shell sdl2-config --libs) -lSDL2_image -lSDL2_ttf
MATH_LIB = -lm

TARGET_GRID = grid_extract
GRID_SRCS = grid_extract.c
GRID_OBJS = $(GRID_SRCS:.c=.o)

TARGET_SOLVER = solver
SOLVER_SRCS = solver.c
SOLVER_OBJS = $(SOLVER_SRCS:.c=.o)

TARGET_XOR = xor
XOR_SRCS = xor.c
XOR_OBJS = $(XOR_SRCS:.c=.o)

TARGET_UI= test
UI_SRCS=main.c UI.c
UI_OBJS=${UI_SRCS:.c=.o}

TARGET_PROG= programme
PROG_SRCS = programme.c reseaux.c
PROG_OBJS=${PROG_SRCS:.c=.o}

all: $(TARGET_GRID) $(TARGET_SOLVER) $(TARGET_XOR) $(TARGET_UI) $(TARGET_PROG)

$(TARGET_GRID): $(GRID_OBJS)
	$(CC) $(CFLAGS) -o $(TARGET_GRID) $(GRID_OBJS) $(SDL_LIBS) $(MATH_LIB)

$(TARGET_SOLVER): $(SOLVER_OBJS)
	$(CC) $(CFLAGS) -o $(TARGET_SOLVER) $(SOLVER_OBJS)

$(TARGET_XOR): $(XOR_OBJS)
	$(CC) $(CFLAGS) -o $(TARGET_XOR) $(XOR_OBJS) $(MATH_LIB)

$(TARGET_UI): $(UI_OBJS)
	$(CC) $(LDFLAGS) $(UI_OBJS) $(SDL_LIBS) -o $@

$(TARGET_PROG): $(PROG_OBJS)
	$(CC) $(LDFLAGS) -o $(TARGET_PROG) $(PROG_OBJS) $(SDL_LIBS) $(MATH_LIB)

%.o: %.c
	$(CC) $(CFLAGS) $(SDL_CFLAGS) -c $< -o $@

clean:
	rm -f $(GRID_OBJS) $(TARGET_GRID) $(SOLVER_OBJS) $(TARGET_SOLVER) $(XOR_OBJS) $(TARGET_XOR) $(UI_OBJS) $(TARGET_UI) $(PROG_OBJS) $(TARGET_PROG) 

extra-clean:
	rm -f grid_r*.bmp word_*.bmp grid*.bmp words*.bmp step*.bmp mask*.bmp

.PHONY: all clean extra-clean


