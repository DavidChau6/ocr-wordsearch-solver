#ifndef RESULTAT_H
#define RESULTAT_H

#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>


#define LINE_R 255
#define LINE_G 0
#define LINE_B 0
#define LINE_A 255


#define LINE_THICKNESS 4


typedef struct {
    int x1, y1;
    int x2, y2;
} WordCoord;


int read_coords(const char *coord_file, WordCoord **coords_out);


int draw_words_and_save(const char *input_file,
                        WordCoord *coords,
                        int num_words,
                        const char *output_file);

#endif

