#ifndef RESULTAT_H
#define RESULTAT_H

#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>

#define LINE_R 255
#define LINE_G 0
#define LINE_B 0
#define LINE_A 255


#define LINE_THICKNESS 2


typedef struct {
    int x1, y1;
    int x2, y2;
} WordCoord;


int read_coords(const char *coord_file, WordCoord **coords_out);
Uint32 get_pixel_raw(SDL_Surface *surf, int x, int y);
void set_pixel_raw(SDL_Surface *surf, int x, int y, Uint32 val);
Uint8 get_gray(SDL_Surface *surf, int x, int y);
void color_to_mask(SDL_Surface *img, Uint8 *mask);
static void draw_thick_line(SDL_Surface *surface,
                            int x1, int y1,
                            int x2, int y2,
                            int thickness);
void draw_box(SDL_Surface *s, int x1,int y1,int x2,int y2,int t, int t1);
void find_mask_bbox(Uint8 *mask, int w, int h,
                    int *x0, int *y0, int *x1, int *y1);


int draw_words_and_save(const char *input_file,
                        WordCoord *coords,
                        int num_words,
                        const char *output_file);

#endif

