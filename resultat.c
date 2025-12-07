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


    SDL_Surface *surface =
        SDL_ConvertSurfaceFormat(loaded, SDL_PIXELFORMAT_ARGB8888, 0);
    SDL_FreeSurface(loaded);
    if (!surface) {
        fprintf(stderr, "Erreur conversion surface : %s\n", SDL_GetError());
        SDL_Quit();
        return -1;
    }

    int img_w = surface->w;
    int img_h = surface->h;


    int max_x = -1, max_y = -1;
    for (int i = 0; i < num_words; i++) {
        if (coords[i].x1 > max_x) max_x = coords[i].x1;
        if (coords[i].x2 > max_x) max_x = coords[i].x2;
        if (coords[i].y1 > max_y) max_y = coords[i].y1;
        if (coords[i].y2 > max_y) max_y = coords[i].y2;
    }
    int grid_cols = max_x + 1;
    int grid_rows = max_y + 1;



    Uint8 r,g,b,a;
    int min_x = img_w, max_xp = -1;
    int min_y = img_h, max_yp = -1;

    for (int y = 0; y < img_h; y++) {
        Uint32 *p = (Uint32 *)((Uint8 *)surface->pixels + y * surface->pitch);
        for (int x = 0; x < img_w; x++) {
            SDL_GetRGBA(p[x], surface->format, &r, &g, &b, &a);
            if (!(r > 250 && g > 250 && b > 250)) {  
                if (x < min_x) min_x = x;
                if (x > max_xp) max_xp = x;
                if (y < min_y) min_y = y;
                if (y > max_yp) max_yp = y;
            }
        }
    }

    if (max_xp <= min_x || max_yp <= min_y) {
        fprintf(stderr, "Impossible de détecter la zone de grille\n");
        SDL_FreeSurface(surface);
        SDL_Quit();
        return -1;
    }

    
    const int MARGIN = 4;
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

        draw_thick_line(surface, x1, y1, x2, y2, LINE_THICKNESS);
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

