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

TARGET_UI= interface
UI_SRCS= main.c UI.c
UI_OBJS= ${UI_SRCS:.c=.o}

TARGET_PROG= programme
PROG_SRCS = programme.c reseaux.c
PROG_OBJS= ${PROG_SRCS:.c=.o}


TARGET_RESOU = resoudre
RESOU_SRCS = resoudre.c
RESOU_OBJS = $(RESOU_SRCS:.c=.o)

TARGET_RESULT = result
RESULT_SRCS = resultat.c
RESULT_OBJS = $(RESULT_SRCS:.c=.o)



all: $(TARGET_GRID) $(TARGET_SOLVER) $(TARGET_XOR) $(TARGET_UI) $(TARGET_PROG) $(TARGET_RESOU) $(TARGET_RESULT)



$(TARGET_GRID): $(GRID_OBJS)
	$(CC) $(CFLAGS) -o $@ $(GRID_OBJS) $(SDL_LIBS) $(MATH_LIB)

$(TARGET_SOLVER): $(SOLVER_OBJS)
	$(CC) $(CFLAGS) -o $@ $(SOLVER_OBJS)

$(TARGET_XOR): $(XOR_OBJS)
	$(CC) $(CFLAGS) -o $@ $(XOR_OBJS) $(MATH_LIB)

$(TARGET_UI): $(UI_OBJS)
	$(CC) $(LDFLAGS) $(UI_OBJS) $(SDL_LIBS) -o $@

$(TARGET_PROG): $(PROG_OBJS)
	$(CC) $(LDFLAGS) -o $@ $(PROG_OBJS) $(SDL_LIBS) $(MATH_LIB)

$(TARGET_RESOU): $(RESOU_OBJS)
	$(CC) $(CFLAGS) -o $@ $(RESOU_OBJS)


$(TARGET_RESULT): $(RESULT_OBJS)
	$(CC) $(CFLAGS) -o $@ $(RESULT_OBJS) $(SDL_LIBS)



%.o: %.c
	$(CC) $(CFLAGS) $(SDL_CFLAGS) -c $< -o $@

clean:
	rm -f $(GRID_OBJS) $(TARGET_GRID) $(SOLVER_OBJS) $(TARGET_SOLVER) $(XOR_OBJS) $(TARGET_XOR) $(UI_OBJS) $(TARGET_UI) $(PROG_OBJS) $(TARGET_PROG) $(RESOU_OBJS) $(TARGET_RESOU) $(RESULT_OBJS) $(TARGET_RESULT)

extra-clean:
	rm -f grid_r*.bmp word_*.bmp grid*.bmp words*.bmp step*.bmp mask*.bmp output.txt word.txt coord.txt donnees.txt lvl_2_2.bmp

.PHONY: all clean extra-clean

