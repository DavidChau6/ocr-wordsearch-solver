#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
static void save_region(SDL_Surface *img, int x0, int y0, int x1, int y1, const char *name)
{
    if (x1 <= x0 || y1 <= y0) return;
    int w = x1 - x0 + 1, h = y1 - y0 + 1;
    SDL_Surface *out = SDL_CreateRGBSurfaceWithFormat(0, w, h, img->format->BitsPerPixel, img->format->format);

    SDL_LockSurface(img);
    SDL_LockSurface(out);
    for (int yy = 0; yy < h; yy++) {
        Uint8 *dst = (Uint8*)out->pixels + yy * out->pitch;
        Uint8 *src = (Uint8*)img->pixels + (y0 + yy) * img->pitch + x0 * img->format->BytesPerPixel;
        memcpy(dst, src, w * img->format->BytesPerPixel);
    }
    SDL_UnlockSurface(out);
    SDL_UnlockSurface(img);

    SDL_SaveBMP(out, name);
    SDL_FreeSurface(out);
    printf("Zone sauvegardée : %s (%dx%d)\n", name, w, h);
}

void color_to_mask(SDL_Surface *img, Uint8 *mask) {
    int w = img->w, h = img->h;
    Uint32 *pix = img->pixels;
    long rs=0, gs=0, bs=0, n=0;


    for (int y=0; y<h; y++)
        for (int x=0; x<w; x++) {
            Uint8 r,g,b;
            SDL_GetRGB(pix[y*w + x], img->format, &r, &g, &b);
            rs += r; gs += g; bs += b; n++;
        }

    Uint8 br = rs/n, bg = gs/n, bb = bs/n;
    int bg_lum = (br+bg+bb)/3;


    for (int y=0; y<h; y++)
        for (int x=0; x<w; x++) {
            Uint8 r,g,b;
            SDL_GetRGB(pix[y*w + x], img->format, &r, &g, &b);
            int lum = (r+g+b)/3;
            int diff = abs(r - br) + abs(g - bg) + abs(b - bb);
            mask[y*w + x] = (diff > 40 || lum < bg_lum - 30) ? 1 : 0;
        }


    int sum = 0;
    for (int i=0; i<w*h; i++) sum += mask[i];
    double ratio = (double)sum / (w*h);
    if (ratio > 0.5) {
        printf("Inversion automatique du masque (fond noir détecté)\n");
        for (int i=0; i<w*h; i++) mask[i] = !mask[i];
    }

}

void detect_two_blocks(Uint8 *mask, int w, int h,
                       int *gx0,int *gy0,int *gx1,int *gy1,
                       int *wx0,int *wy0,int *wx1,int *wy1)
{
    
    int *colsum = calloc(w, sizeof(int));
    int *rowsum = calloc(h, sizeof(int));

    for (int y=0;y<h;y++)
        for (int x=0;x<w;x++)
            if(mask[y*w+x]) {
                rowsum[y]++;
                colsum[x]++;
            }

   
    int miny=h,maxy=0;
    for(int y=0;y<h;y++)
        if(rowsum[y]>0){ miny=y; break; }
    for(int y=h-1;y>=0;y--)
        if(rowsum[y]>0){ maxy=y; break; }

  
    int minx=w,maxx=0;
    for(int x=0;x<w;x++)
        if(colsum[x]>0){ minx=x; break; }
    for(int x=w-1;x>=0;x--)
        if(colsum[x]>0){ maxx=x; break; }

    int gap_size = (int)(0.05 * w);
    int gap_pos = -1;
    for(int x=minx+10;x<maxx-10;x++){
        int empty=0;
        for(int y=miny;y<=maxy;y++)
            if(mask[y*w+x]) empty++;
        if(empty == 0){
            int run=1;
            while(x+run<maxx && empty==0){
                int e2=0;
                for(int y=miny;y<=maxy;y++)
                    if(mask[y*w+(x+run)]) e2++;
                if(e2==0) run++;
                else break;
            }
            if(run > gap_size){
                gap_pos = x;
                break;
            }
        }
    }

    int gap_hpos = -1;
    for(int y=miny+10;y<maxy-10;y++){
        int empty=0;
        for(int x=minx;x<=maxx;x++)
            if(mask[y*w+x]) empty++;
        if(empty == 0){
            int run=1;
            while(y+run<maxy && empty==0){
                int e2=0;
                for(int x=minx;x<=maxx;x++)
                    if(mask[(y+run)*w+x]) e2++;
                if(e2==0) run++;
                else break;
            }
            if(run > 0.05*h){
                gap_hpos = y;
                break;
            }
        }
    }
    
    int count_black_left = 0, count_black_right = 0;
    int area_left = 0, area_right = 0;

    if (gap_pos != -1) {
    	for (int y = miny; y <= maxy; y++) {
        	for (int x = minx; x <= gap_pos; x++) {
       			area_left++;
      			if (mask[y*w + x]) count_black_left++;
        	}
        	for (int x = gap_pos + 1; x <= maxx; x++) {
            		area_right++;
            		if (mask[y*w + x]) count_black_right++;
        	}
    }

    double dens_left = (double)count_black_left / area_left;
    double dens_right = (double)count_black_right / area_right;

    if (dens_left > dens_right) {
        *gx0 = minx; *gy0 = miny; *gx1 = gap_pos; *gy1 = maxy;
        *wx0 = gap_pos + 1; *wy0 = miny; *wx1 = maxx; *wy1 = maxy;
    } else {
        *gx0 = gap_pos + 1; *gy0 = miny; *gx1 = maxx; *gy1 = maxy;
        *wx0 = minx; *wy0 = miny; *wx1 = gap_pos; *wy1 = maxy;
    }
}

    if (gap_hpos != -1) {
        int count_black_top = 0, count_black_bottom = 0;
        int area_top = 0, area_bottom = 0;

        for (int y = miny; y <= gap_hpos; y++) {
            for (int x = minx; x <= maxx; x++) {
                area_top++;
                if (mask[y*w + x]) count_black_top++;
            }
        }
        for (int y = gap_hpos + 1; y <= maxy; y++) {
            for (int x = minx; x <= maxx; x++) {
                area_bottom++;
                if (mask[y*w + x]) count_black_bottom++;
            }
        }

        double dens_top = (double)count_black_top / area_top;
        double dens_bottom = (double)count_black_bottom / area_bottom;

        if (dens_top > dens_bottom) {
            *gx0 = minx; *gy0 = miny; *gx1 = maxx; *gy1 = gap_hpos;
            *wx0 = minx; *wy0 = gap_hpos + 1; *wx1 = maxx; *wy1 = maxy;
        } else {
            *gx0 = minx; *gy0 = gap_hpos + 1; *gx1 = maxx; *gy1 = maxy;
            *wx0 = minx; *wy0 = miny; *wx1 = maxx; *wy1 = gap_hpos;
        }
    }
    free(colsum);
    free(rowsum);
}

static inline Uint8 get_gray(SDL_Surface *img, int x, int y) {
    Uint8 *p = (Uint8*)img->pixels + y * img->pitch + x * img->format->BytesPerPixel;
    Uint8 r, g, b;
    SDL_GetRGB(*(Uint32*)p, img->format, &r, &g, &b);
    return (Uint8)(0.3*r + 0.59*g + 0.11*b);
}

void color_to_mask2(SDL_Surface *img, Uint8 *mask) {
    int w = img->w, h = img->h;
    int total = 0;
    double mean = 0.0;

    for (int y = 0; y < h; y++)
        for (int x = 0; x < w; x++) {
            Uint8 g = get_gray(img, x, y);
            mean += g;
            total++;
        }
    mean /= total;
    Uint8 thr;
    //printf("_______%f____", mean);
    if (mean > 240)
	    thr = (Uint8)(mean * 0.9);
    else
	    thr = (Uint8)(mean * 0.45); //The I is missing but it is splited, last thing to fix

    for (int y = 0; y < h; y++)
        for (int x = 0; x < w; x++) {
            Uint8 g = get_gray(img, x, y);
            mask[y*w + x] = (g < thr) ? 1 : 0; 
        }
}


void save_mask_as_bmp(Uint8 *mask, int w, int h, const char *filename) {
	SDL_Surface *surf = SDL_CreateRGBSurface(0, w, h, 24, 0x0000FF, 0x00FF00, 0xFF0000, 0);
	if (!surf) {
	fprintf(stderr, "Erreur SDL_CreateRGBSurface: %s\n", SDL_GetError());
	return;
	}
	Uint8 *pixels = surf->pixels;
	int pitch = surf->pitch;

	for (int y = 0; y < h; y++) {
    		for (int x = 0; x < w; x++) {
        		Uint8 v = mask[y * w + x] ? 0 : 255;
        		Uint8 *p = pixels + y * pitch + x * 3;
        		p[0] = v;
        		p[1] = v;
        		p[2] = v; 
    		}
	}

	if (SDL_SaveBMP(surf, filename) != 0)
    		fprintf(stderr, "Erreur SDL_SaveBMP(%s): %s\n", filename, SDL_GetError());

	SDL_FreeSurface(surf);
}

void split_letters(SDL_Surface *img, const char *prefix) {
    int w = img->w, h = img->h;
    Uint8 *mask = calloc(w * h, 1);
    color_to_mask(img, mask);
    
    int *row = calloc(h, sizeof(int));
    for (int y = 0; y < h; y++)
        for (int x = 0; x < w; x++)
            if (mask[y*w + x]) row[y]++;

    int ystart[200], yend[200];
    int thr = w * 0.003;
    int in = 0, s = 0, nby = 0;
    for (int y = 0; y < h; y++) {
        if (row[y] > thr) {
            if (!in) { s = y; in = 1; }
        } else if (in) {
            ystart[nby] = s;
            yend[nby] = y;
            nby++;
            in = 0;
        }
    }
    color_to_mask2(img, mask);
    save_mask_as_bmp(mask, w, h, "words_mask.bmp");
    int *qx = malloc(w * h * sizeof(int));
    int *qy = malloc(w * h * sizeof(int));
    for (int m = 0; m < nby; m++) {
        int y0 = ystart[m], y1 = yend[m];
        int nb_letters = 0;

        for (int y = y0; y <= y1; y++) {
            for (int x = 0; x < w; x++) {
                if (!mask[y*w + x]) continue;

                int minx = x, maxx = x, miny = y, maxy = y;
                int qh = 0, qt = 0;
                qx[qt] = x; qy[qt] = y; qt++;
                mask[y*w + x] = 0;

                while (qh < qt) {
                    int cx = qx[qh], cy = qy[qh]; qh++;
                    const int dx[4] = {1,-1,0,0};
                    const int dy[4] = {0,0,1,-1};
                    for (int k = 0; k < 4; k++) {
                        int nx = cx + dx[k], ny = cy + dy[k];
                        if (nx < 0 || nx >= w || ny < y0 || ny > y1) continue;
                        if (mask[ny*w + nx]) {
                            mask[ny*w + nx] = 0;
                            qx[qt] = nx; qy[qt] = ny; qt++;
                            if (nx < minx) minx = nx;
                            if (nx > maxx) maxx = nx;
                            if (ny < miny) miny = ny;
                            if (ny > maxy) maxy = ny;
                        }
                    }
                }

            if (maxx - minx < 3 || maxy - miny < 3) continue;

            char name[128];
            snprintf(name, sizeof(name),
                     "%s_r%02d_c%02d.bmp", prefix, m, nb_letters++);
            save_region(img, minx, miny, maxx, maxy, name);
            }
        }
    }

    free(mask);
    free(qx);
    free(qy);
    free(row);
}

void split_grid_letters(SDL_Surface *img, const char *prefix) {
    int w = img->w, h = img->h;
    Uint8 *mask = calloc(w*h, 1);
    color_to_mask(img, mask);
    save_mask_as_bmp(mask, w, h, "grid_mask.bmp");
 
    int minx=w, maxx=0, miny=h, maxy=0;
    for (int y=0;y<h;y++)
        for (int x=0;x<w;x++)
            if (mask[y*w+x]) {
                if (x<minx) minx=x;
                if (x>maxx) maxx=x;
                if (y<miny) miny=y;
                if (y>maxy) maxy=y;
            }

    int grid_w = maxx - minx + 1;
    int grid_h = maxy - miny + 1;
    printf("Zone utile : (%d,%d)-(%d,%d) => %dx%d\n", minx, miny, maxx, maxy, grid_w, grid_h);


    int *col = calloc(grid_w, sizeof(int));
    int *row = calloc(grid_h, sizeof(int));

    for (int y=miny;y<=maxy;y++)
        for (int x=minx;x<=maxx;x++)
            if (mask[y*w+x]) {
                col[x-minx]++;
                row[y-miny]++;
            }


    int col_thr = grid_h * 0.006;
    int row_thr = grid_w * 0.004;


    int xstart[300], xend[300], ystart[300], yend[300];
    int nbx=0, nby=0;
    int inband=0, s=0;

    for (int x=0;x<grid_w;x++) {
        if (col[x] > col_thr) {
            if (!inband) { s=x; inband=1; }
        } else if (inband) {
            if (x-s > 5) { 
                xstart[nbx]=s; xend[nbx]=x-1; nbx++;
            }
            inband=0;
        }
    }
    if (inband && (grid_w - s > 5)) {
        xstart[nbx]=s;
        xend[nbx]=grid_w-1;
        nbx++;
    }
    inband=0;
    for (int y=0;y<grid_h;y++) {
        if (row[y] > row_thr) {
            if (!inband) { s=y; inband=1; }
        } else if (inband) {
            if (y-s > 5) { 
                ystart[nby]=s; yend[nby]=y-1; nby++;
            }
            inband=0;
        }
    }
    if (inband && (grid_h - s > 5)) {
        ystart[nby]=s;
        yend[nby]=grid_h-1;
        nby++;
    }
   
    int merged_nbx=0, merged_nby=0;
    for (int i=0; i<nbx; i++) {
        if (merged_nbx==0 || xstart[i] - xend[merged_nbx-1] > 3) {
            xstart[merged_nbx]=xstart[i];
            xend[merged_nbx]=xend[i];
            merged_nbx++;
        } else {
            xend[merged_nbx-1]=xend[i];
        }
    }

    for (int i=0; i<nby; i++) {
        if (merged_nby==0 || ystart[i] - yend[merged_nby-1] > 3) {
            ystart[merged_nby]=ystart[i];
            yend[merged_nby]=yend[i];
            merged_nby++;
        } else {
            yend[merged_nby-1]=yend[i];
        }
    }

    printf("Grille : %d colonnes × %d lignes (fusionnées)\n", merged_nbx, merged_nby);

    if (merged_nbx < 2 || merged_nby < 2) {
        printf("Détection échouée.\n");
        free(mask); free(col); free(row);
        return;
    }

    for (int r=0; r<merged_nby; r++) {
        for (int c=0; c<merged_nbx; c++) {
            int x0 = minx + xstart[c];
            int x1 = minx + xend[c];
            int y0 = miny + ystart[r];
            int y1 = miny + yend[r];
            char name[128];
            snprintf(name, sizeof(name), "%s_r%02d_c%02d.bmp", prefix, r, c);
            save_region(img, x0, y0, x1, y1, name);
        }
    }

    free(mask);
    free(col);
    free(row);
}



SDL_Surface* rotateSurface(SDL_Surface* src, double angle)
{

    double rad = angle * M_PI / 180.0;
    int w = src->w;
    int h = src->h;
    double cosA = cos(rad);
    double sinA = sin(rad);

    int newW = fabs(w * cosA) + fabs(h * sinA);
    int newH = fabs(w * sinA) + fabs(h * cosA);

    SDL_Surface* dst = SDL_CreateRGBSurface(
        0, newW, newH,
        src->format->BitsPerPixel,
        src->format->Rmask, src->format->Gmask,
        src->format->Bmask, src->format->Amask
    );
    if (!dst) {
        fprintf(stderr, "Erreur SDL_CreateRGBSurface: %s\n", SDL_GetError());
        return NULL;
    }

    
    if (SDL_FillRect(dst, NULL, SDL_MapRGB(dst->format, 255, 255, 255)) != 0)
        fprintf(stderr, "Erreur SDL_FillRect: %s\n", SDL_GetError());

    Uint32* srcPixels = (Uint32*)src->pixels;
    Uint32* dstPixels = (Uint32*)dst->pixels;

    int cx = w / 2;
    int cy = h / 2;
    int ncx = newW / 2;
    int ncy = newH / 2;

    for (int y = 0; y < newH; y++) {
        for (int x = 0; x < newW; x++) {
            int oldX = (int)((x - ncx) * cosA + (y - ncy) * sinA + cx);
            int oldY = (int)(-(x - ncx) * sinA + (y - ncy) * cosA + cy);

            if (oldX >= 0 && oldX < w && oldY >= 0 && oldY < h)
                dstPixels[y * newW + x] = srcPixels[oldY * w + oldX];
            
        }
    }

    return dst;
}

int main(int argc, char **argv)
{
    if (argc < 3) {
        printf("Usage: %s <image> <angle>\n", argv[0]);
        return 1;
    }

    const char* filename = argv[1];
    double angle = atof(argv[2]);

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "Erreur SDL_Init: %s\n", SDL_GetError());
        return 1;
    }

    int flags = IMG_INIT_PNG | IMG_INIT_JPG;
    if ((IMG_Init(flags) & flags) != flags) {
        fprintf(stderr, "Erreur IMG_Init: %s\n", IMG_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Surface* img = IMG_Load(filename);
    if (!img) {
        fprintf(stderr, "Erreur chargement image %s: %s\n", filename, IMG_GetError());
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    
    SDL_Surface* converted = SDL_ConvertSurfaceFormat(img, SDL_PIXELFORMAT_RGBA8888, 0);
    SDL_FreeSurface(img);
    if (!converted) {
        fprintf(stderr, "Erreur conversion image\n");
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    SDL_Surface* rotated = rotateSurface(converted, angle);
    SDL_FreeSurface(converted);
    if (!rotated) {
        fprintf(stderr, "Erreur rotation\n");
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    
    int w = rotated->w;
    int h = rotated->h;
    Uint8* mask = calloc(w * h, 1);
    if (!mask) {
        fprintf(stderr, "Erreur allocation masque\n");
        SDL_FreeSurface(rotated);
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    color_to_mask(rotated, mask);

    
    int gx0, gy0, gx1, gy1;
    int wx0, wy0, wx1, wy1;
    detect_two_blocks(mask, w, h, &gx0, &gy0, &gx1, &gy1, &wx0, &wy0, &wx1, &wy1);

    printf("Grille: (%d,%d)-(%d,%d)\n", gx0, gy0, gx1, gy1);
    printf("Mots : (%d,%d)-(%d,%d)\n", wx0, wy0, wx1, wy1);

    
    SDL_Rect grid_rect = {gx0, gy0, gx1-gx0+1, gy1-gy0+1};
    SDL_Rect word_rect = {wx0, wy0, wx1-wx0+1, wy1-wy0+1};

    SDL_Surface* grid_img = SDL_CreateRGBSurfaceWithFormat(0, grid_rect.w, grid_rect.h,
                                                           rotated->format->BitsPerPixel,
                                                           rotated->format->format);
    SDL_BlitSurface(rotated, &grid_rect, grid_img, NULL);

    SDL_Surface* word_img = SDL_CreateRGBSurfaceWithFormat(0, word_rect.w, word_rect.h,
                                                           rotated->format->BitsPerPixel,
                                                           rotated->format->format);
    SDL_BlitSurface(rotated, &word_rect, word_img, NULL);

    
    split_grid_letters(grid_img, "grid_rot");
    split_letters(word_img, "word_rot");

    
    SDL_SaveBMP(grid_img, "grid_only.bmp");
    SDL_SaveBMP(word_img, "words_only.bmp");

   
    free(mask);
    SDL_FreeSurface(grid_img);
    SDL_FreeSurface(word_img);
    SDL_FreeSurface(rotated);

    IMG_Quit();
    SDL_Quit();

    return 0;
}
