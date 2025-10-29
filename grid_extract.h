#ifndef GRID_EXTRACT_H
#define GRID_EXTRACT_H
static void save_region(SDL_Surface *img, int x0, int y0, int x1, int y1, const char *name);
void color_to_mask(SDL_Surface *img, Uint8 *mask);
int find_bands(int *proj, int len, int thr, int *starts, int *ends, int maxbands);
void detect_two_blocks(Uint8 *mask, int w, int h,
                       int *gx0,int *gy0,int *gx1,int *gy1,
                       int *wx0,int *wy0,int *wx1,int *wy1);
void split_letters(SDL_Surface *img, const char *prefix);
void split_grid_letters(SDL_Surface *img, const char *prefix);
#endif
