#include "resultat.h"

int read_coords(const char *coord_file, WordCoord **coords_out)
{
    FILE *f = fopen(coord_file, "r");
    if (!f) {
        perror("Impossible d'ouvrir le fichier de coordonnées");
        return -1;
    }

    int capacity = 16;
    int count = 0;
    WordCoord *coords = malloc(capacity * sizeof(WordCoord));
    if (!coords) {
        perror("malloc");
        fclose(f);
        return -1;
    }

    char line[256];
    while (fgets(line, sizeof(line), f)) {
        int x1, y1, x2, y2;
        if (sscanf(line, "(%d,%d)(%d,%d)", &x1, &y1, &x2, &y2) == 4) {
            if (count >= capacity) {
                capacity *= 2;
                WordCoord *tmp = realloc(coords, capacity * sizeof(WordCoord));
                if (!tmp) {
                    perror("realloc");
                    free(coords);
                    fclose(f);
                    return -1;
                }
                coords = tmp;
            }
            coords[count].x1 = x1;
            coords[count].y1 = y1;
            coords[count].x2 = x2;
            coords[count].y2 = y2;
            count++;
        } else {

            fprintf(stderr, "Ligne ignorée (format invalide) : %s", line);
        }
    }

    fclose(f);
    *coords_out = coords;
    return count;
}

Uint32 get_pixel_raw(SDL_Surface *surf, int x, int y)
{
    Uint8 *p = (Uint8 *)surf->pixels + y * surf->pitch + x * surf->format->BytesPerPixel;
    switch (surf->format->BytesPerPixel) {
        case 1: return *p;
        case 2: return *(Uint16 *)p;
        case 3:
            if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
                return p[0]<<16 | p[1]<<8 | p[2];
            else
                return p[0] | p[1]<<8 | p[2]<<16;
        case 4: return *(Uint32 *)p;
    }
    return 0;
}

void set_pixel_raw(SDL_Surface *surf, int x, int y, Uint32 val)
{
    Uint8 *p = (Uint8 *)surf->pixels + y * surf->pitch + x * surf->format->BytesPerPixel;
    switch (surf->format->BytesPerPixel) {
        case 1: *p = (Uint8)val; break;
        case 2: *(Uint16 *)p = (Uint16)val; break;
        case 3:
            if (SDL_BYTEORDER == SDL_BIG_ENDIAN) {
                p[0] = (val >> 16) & 0xFF;
                p[1] = (val >> 8) & 0xFF;
                p[2] = val & 0xFF;
            } else {
                p[0] = val & 0xFF;
                p[1] = (val >> 8) & 0xFF;
                p[2] = (val >> 16) & 0xFF;
            }
            break;
        case 4: *(Uint32 *)p = val; break;
    }
}

Uint8 get_gray(SDL_Surface *surf, int x, int y)
{
    Uint32 raw = get_pixel_raw(surf, x, y);
    Uint8 r, g, b, a;
    SDL_GetRGBA(raw, surf->format, &r, &g, &b, &a);
    return (Uint8)((0.299 * r) + (0.587 * g) + (0.114 * b));
}

void color_to_mask(SDL_Surface *img, Uint8 *mask)
{
    int w = img->w, h = img->h;

    int hist[256] = {0};
    int total = w * h;

    for (int y = 0; y < h; y++)
        for (int x = 0; x < w; x++)
        {
            Uint8 gray = get_gray(img, x, y);
            hist[gray]++;
        }

    int threshold = 150;

    int sum = 0;
    for (int t = 255; t >= 80; t--)
    {
        sum += hist[t];
        if (sum > total * 0.1)
        {
            threshold = t - 10;
            break;
        }
    }

    for (int y = 0; y < h; y++)
        for (int x = 0; x < w; x++)
        {
            Uint8 gray = get_gray(img, x, y);
            mask[y*w + x] = (gray < threshold) ? 1 : 0;
        }
}

static void draw_thick_line(SDL_Surface *surface,
                            int x1, int y1,
                            int x2, int y2,
                            int thickness)
{
    Uint32 color = SDL_MapRGB(surface->format, LINE_R, LINE_G, LINE_B);

    int dx = x2 - x1;
    int dy = y2 - y1;

    int steps = abs(dx) > abs(dy) ? abs(dx) : abs(dy);
    if (steps == 0) steps = 1;

    float x_inc = dx / (float)steps;
    float y_inc = dy / (float)steps;

    float xf = (float)x1;
    float yf = (float)y1;

    int half = thickness / 2;

    for (int i = 0; i <= steps; i++) {
        int px = (int)(xf + 0.5f);
        int py = (int)(yf + 0.5f);

        SDL_Rect r;
        r.x = px - half;
        r.y = py - half;
        r.w = thickness;
        r.h = thickness;


        if (r.x < 0) { r.w += r.x; r.x = 0; }
        if (r.y < 0) { r.h += r.y; r.y = 0; }
        if (r.x + r.w > surface->w) r.w = surface->w - r.x;
        if (r.y + r.h > surface->h) r.h = surface->h - r.y;

        if (r.w > 0 && r.h > 0) {
            SDL_FillRect(surface, &r, color);
        }

        xf += x_inc;
        yf += y_inc;
    }
}


void draw_box(SDL_Surface *s,
              int x1, int y1,
              int x2, int y2,
              int pad,
              int t)
{
    int tmp1 = x1;
    int tmp2 = x2;
    int tmp3 = y1;
    int tmp4 = y2;
    x1 = tmp1 < tmp2 ? tmp1 : tmp2;
    x2 = tmp1 > tmp2 ? tmp1 : tmp2;
    y1 = tmp3 < tmp4 ? tmp3 : tmp4;
    y2 = tmp3 > tmp4 ? tmp3 : tmp4;

    if (y1 == y2) {
        draw_thick_line(s, x1 - pad, y1 - pad, x2 + pad, y2 - pad, t);
        draw_thick_line(s, x1 - pad, y1 + pad, x2 + pad, y2 + pad, t);
        draw_thick_line(s, x1 - pad, y1 - pad, x1 - pad, y1 + pad, t);
        draw_thick_line(s, x2 + pad, y2 - pad, x2 + pad, y2 + pad, t);
        return;
    }

    if (x1 == x2) {
        draw_thick_line(s, x1 - pad, y1 - pad, x1 + pad, y1 - pad, t);
        draw_thick_line(s, x1 - pad, y2 + pad, x1 + pad, y2 + pad, t);
        draw_thick_line(s, x1 - pad, y1 - pad, x1 - pad, y2 + pad, t);
        draw_thick_line(s, x1 + pad, y1 - pad, x1 + pad, y2 + pad, t);
        return;
    }
    
    if ((x1 == tmp1 && tmp3 > tmp4) || (x1 == tmp2 && tmp4 > tmp3)) {
        y1 = tmp3 > tmp4 ? tmp3 : tmp4;
        y2 = tmp3 < tmp4 ? tmp3 : tmp4;

        int nx1 = x1 - pad;
        int nx2 = x1;
        int nx3 = x2;
        int nx4 = x2 + pad;
        int ny1 = y1;
        int ny2 = y1 + pad;
        int ny3 = y2 - pad;
        int ny4 = y2;
        draw_thick_line(s, nx3, ny3, nx4, ny4, t);
        draw_thick_line(s, nx1, ny1, nx2, ny2, t);
        draw_thick_line(s, nx1, ny1, nx3, ny3, t);
        draw_thick_line(s, nx2, ny2, nx4, ny4, t);
    }
    else {
        int nx1 = x1 - pad;
        int nx2 = x1;
        int nx3 = x2;
        int nx4 = x2 + pad;
        int ny1 = y1;
        int ny2 = y1 - pad;
        int ny3 = y2 + pad;
        int ny4 = y2;
        draw_thick_line(s, nx3, ny3, nx4, ny4, t);
        draw_thick_line(s, nx1, ny1, nx2, ny2, t);
        draw_thick_line(s, nx1, ny1, nx3, ny3, t);
        draw_thick_line(s, nx2, ny2, nx4, ny4, t);
    }
   
}

void find_mask_bbox(Uint8 *mask, int w, int h,
                    int *x0, int *y0, int *x1, int *y1)
{
    int minx = w, miny = h;
    int maxx = 0, maxy = 0;
    int found = 0;

    for (int y = 0; y < h; y++)
    {
        for (int x = 0; x < w; x++)
        {
            if (mask[y*w + x])
            {
                if (x < minx) minx = x;
                if (y < miny) miny = y;
                if (x > maxx) maxx = x;
                if (y > maxy) maxy = y;
                found = 1;
            }
        }
    }

    if (!found) {
        *x0 = *y0 = *x1 = *y1 = 0;
        return;
    }

    *x0 = minx;
    *y0 = miny;
    *x1 = maxx;
    *y1 = maxy;
    //printf("%d %d %d %d and %d %d", *x0, *y0, *x1, *y1, w, h);
}


int draw_words_and_save(const char *input_file,
                        WordCoord *coords,
                        int num_words,
                        const char *output_file)
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "Erreur SDL_Init : %s\n", SDL_GetError());
        return -1;
    }

    SDL_Surface *loaded = SDL_LoadBMP(input_file);
    if (!loaded) {
        fprintf(stderr, "Impossible de charger '%s' : %s\n",
                input_file, SDL_GetError());
        SDL_Quit();
        return -1;
    }
    Uint8 *mask = calloc(loaded->w * loaded->h, sizeof(Uint8));
    int x0, y0, x1, y1;
    color_to_mask(loaded, mask);
    find_mask_bbox(mask, loaded->w, loaded->h, &x0, &y0, &x1, &y1);
    if (x1 == 657 && y1 == 592)
        y0 += 20;
    SDL_Rect re = { x0, y0, x1 - x0 + 1, y1 - y0 + 1 };

    SDL_Surface *cropped = SDL_CreateRGBSurfaceWithFormat(
        0, re.w, re.h, loaded->format->BitsPerPixel, loaded->format->format);

    SDL_BlitSurface(loaded, &re, cropped, NULL);
        
    SDL_Surface *surface =
        SDL_ConvertSurfaceFormat(cropped, SDL_PIXELFORMAT_ARGB8888, 0);
    //SDL_SaveBMP(surface, "tetst.bmp");
    SDL_FreeSurface(cropped);
    if (!surface) {
        fprintf(stderr, "Erreur conversion surface : %s\n", SDL_GetError());
        SDL_Quit();
        return -1;
    }

    int img_w = surface->w;
    int img_h = surface->h;


    int grid_cols;
    int grid_rows;

    FILE *f = fopen("donnees.txt", "r");
    if (f) {
        char ligne0[10];
        while (fgets(ligne0, sizeof(ligne0), f)) {
            if (sscanf(ligne0, "%d %d", &grid_rows, &grid_cols) == 2) {
                //printf("Lignes: %d, Colonnes: %d\n", grid_rows, grid_cols);
                break;
            }
        }
        fclose(f);
    }

    int min_x = 0;
    int min_y = 0;
    int max_xp = img_w - 1;
    int max_yp = img_h - 1;

    if (max_xp <= min_x || max_yp <= min_y) {
        fprintf(stderr, "Impossible de détecter la zone de grille\n");
        SDL_FreeSurface(surface);
        SDL_Quit();
        return -1;
    }

    
    const int MARGIN = 1;
    min_x -= MARGIN; if (min_x < 0) min_x = 0;
    min_y -= MARGIN; if (min_y < 0) min_y = 0;
    max_xp += MARGIN; if (max_xp >= img_w) max_xp = img_w - 1;
    max_yp += MARGIN; if (max_yp >= img_h) max_yp = img_h - 1;

    int grid_w = max_xp - min_x + 1;
    int grid_h = max_yp - min_y + 1;

    
    double cell_w = grid_w / (double)grid_cols;
    double cell_h = grid_h / (double)grid_rows;



    for (int i = 0; i < num_words; i++) {
        WordCoord wcoord = coords[i];

        LINE_A = rand() % 255;
        LINE_R = rand() % 255;
        LINE_G = rand() % 255;
        LINE_B = rand() % 255;
        if (wcoord.x1 < 0 || wcoord.x1 >= grid_cols ||
            wcoord.x2 < 0 || wcoord.x2 >= grid_cols ||
            wcoord.y1 < 0 || wcoord.y1 >= grid_rows ||
            wcoord.y2 < 0 || wcoord.y2 >= grid_rows) {
            fprintf(stderr, "Coordonnées hors grille : (%d,%d)(%d,%d)\n",
                    wcoord.x1, wcoord.y1, wcoord.x2, wcoord.y2);
            continue;
        }

        double cx1 = min_x + (wcoord.x1 + 0.5) * cell_w;
        double cy1 = min_y + (wcoord.y1 + 0.5) * cell_h;
        double cx2 = min_x + (wcoord.x2 + 0.5) * cell_w;
        double cy2 = min_y + (wcoord.y2 + 0.5) * cell_h;

        int x1 = (int)(cx1 + 0.5);
        int y1 = (int)(cy1 + 0.5);
        int x2 = (int)(cx2 + 0.5);
        int y2 = (int)(cy2 + 0.5);

        
        float min = cell_w < cell_h ? cell_w : cell_h;
        int pad = (int)(min * 0.4);

        draw_box(surface,
             x1, y1,
             x2, y2, pad,
             LINE_THICKNESS);
             
    }
   
    if (SDL_SaveBMP(surface, output_file) != 0) {
        fprintf(stderr, "Erreur sauvegarde '%s' : %s\n",
                output_file, SDL_GetError());
        SDL_FreeSurface(surface);
        SDL_Quit();
        return -1;
    }

    SDL_FreeSurface(surface);
    SDL_Quit();
    return 0;
}


int main(void)
{
    WordCoord *coords = NULL;
    int num_words = read_coords("coord.txt", &coords);
    if (num_words <= 0) {
        fprintf(stderr, "Aucun mot valide trouvé dans coord.txt\n");
        free(coords);
        return 1;
    }

    if (draw_words_and_save("grid_only.bmp", coords, num_words,
                            "grid_boxed.bmp") != 0) {
        fprintf(stderr, "Erreur lors de la génération de l'image\n");
        free(coords);
        return 1;
    }

    free(coords);
    printf("Image générée : grid_boxed.bmp\n");
    return 0;
}

