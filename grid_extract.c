#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

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

void split_letters(SDL_Surface *img, const char *prefix) {
    int w = img->w, h = img->h;
    Uint8 *mask = calloc(w * h, 1);

    color_to_mask2(img, mask);

    int *qx = malloc(w * h * sizeof(int));
    int *qy = malloc(w * h * sizeof(int));

    int nb_letters = 0;
    for (int y = 0; y < h; y++) {
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
                    if (nx < 0 || nx >= w || ny < 0 || ny >= h) continue;
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
            snprintf(name, sizeof(name), "%s_letter_%03d.bmp", prefix, nb_letters++);
            save_region(img, minx, miny, maxx, maxy, name);
        }
    }

    free(mask);
    free(qx);
    free(qy);
}

void split_grid_letters(SDL_Surface *img, const char *prefix) {
    int w = img->w, h = img->h;
    Uint8 *mask = calloc(w*h, 1);
    color_to_mask(img, mask);

 
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







int main(int argc, char **argv) {
    if (argc < 2)
	    return 1;
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Surface *img = IMG_Load(argv[1]);
    Uint8 *mask = calloc(img->w * img->h, 1);
    color_to_mask(img, mask);

    int gx0,gy0,gx1,gy1,wx0,wy0,wx1,wy1;
    detect_two_blocks(mask,img->w,img->h,&gx0,&gy0,&gx1,&gy1,&wx0,&wy0,&wx1,&wy1);

    save_region(img,gx0,gy0,gx1,gy1,"grid.bmp");
    save_region(img,wx0,wy0,wx1,wy1,"words.bmp");

    SDL_Surface *grid = IMG_Load("grid.bmp");
    SDL_Surface *words = IMG_Load("words.bmp");

    split_grid_letters(grid, "grid");
    split_letters(words, "word");

    SDL_FreeSurface(img);
    SDL_FreeSurface(grid);
    SDL_FreeSurface(words);
    free(mask);
    SDL_Quit();
}
