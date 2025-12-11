#ifndef GRID_EXTRACT_H
#define GRID_EXTRACT_H

extern int rows_res;
extern int cols_res;

void save_region(SDL_Surface *img, int x0, int y0, int x1, int y1,
                        const char *name);
void color_to_mask(SDL_Surface *img, Uint8 *mask);
void detect_two_blocks(Uint8 *mask, int w, int h, int *gx0, int *gy0, int *gx1,
                       int *gy1, int *wx0, int *wy0, int *wx1, int *wy1);
Uint32 get_pixel_raw(SDL_Surface *surf, int x, int y);
void set_pixel_raw(SDL_Surface *surf, int x, int y, Uint32 val);
Uint8 get_gray(SDL_Surface *surf, int x, int y);
void convert_to_grayscale(SDL_Surface* surface);
void color_to_mask2(SDL_Surface *img, Uint8 *mask);
void save_mask_as_bmp(Uint8 *mask, int w, int h, const char *filename);
void remove_border_connected(Uint8 *mask, int w, int h);
SDL_Surface* resize_surface(SDL_Surface *src, int new_w, int new_h);
void split_letters(SDL_Surface *img, const char *prefix);
int compare(const void *a, const void *b);
SDL_Surface* median_filter(SDL_Surface* surface);
double estimate_grid_angle(SDL_Surface* surface);
SDL_Surface* rotate_surface(SDL_Surface* surface, double angle_deg);
SDL_Surface* straighten_grid(SDL_Surface* surface);

#endif
