#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include "reseaux.h"

double* load_image(const char* filename, int* w, int* h)
{

        SDL_Surface* img = SDL_LoadBMP(filename);

        if (!img)
        {
                printf("Erreur SDL: %s\n", filename);
                return NULL;
        }

        *w = img->w;
        *h = img->h;
        double* v = malloc(sizeof(double) * (*w) * (*h));
        SDL_LockSurface(img);
        for (int y = 0; y < *h; y++)
        {
                for (int x = 0; x < *w; x++)
                {
                        Uint8 r, g, b;
                        Uint32 px = ((Uint32*)img->pixels)[y * img->pitch / 4 + x];
                        SDL_GetRGB(px, img->format, &r, &g, &b);
                        v[y * (*w) + x] = 1.0 - (r / 255.0);
                }
        }
        SDL_UnlockSurface(img);
        SDL_FreeSurface(img);
        return v;
}

int main()
{
        SDL_Init(SDL_INIT_VIDEO);

        if (!load_model("model.bin"))
        {
                printf("Aucun modèle trouvé. Entraînement...\n");

                init_network();

                for (int epoch = 0; epoch < 1000; epoch++)
                {
                        for (int i = 1; i <= 156; i++)
                        {
                                char filename[128];
                                sprintf(filename, "ref_test2/ref_%i.bmp", i);
                                int w, h;
                                double* img = load_image(filename, &w, &h);
                                if (!img) continue;
                                train_sample(img, (i-1)/6);
                                free(img);
                        }
                }
                save_model("model.bin");
                printf("Entraînement terminé. Modèle sauvegardé.\n");
        }
        else
        {
                printf("Modèle chargé.\n");
        }
        FILE* out = fopen("output.txt", "w");
        if (!out)
        {
                printf("Impossible de créer output.txt\n");
                return 1;
        }

        int r = 0;
        while (1)
        {
                int c = 0;
                int ligne_existante = 0;

                while (1)
                {
                        char fname[64];
                        sprintf(fname, "grid_rot_r%02d_c%02d.bmp", r, c);

                        int w, h;
                        if (!SDL_LoadBMP(fname)) break;

                        double* img = load_image(fname, &w, &h);
                        if (!img) break;

                        char letter = predict_letter(img);
                        free(img);

                        printf("%s -> %c\n", fname, letter);
                        fprintf(out, "%c", letter);

                        c++;
                        ligne_existante = 1;
                }

                if (!ligne_existante) break;

                fprintf(out, "\n");
                r++;
        }
	fclose(out);
        FILE* word = fopen("word.txt", "w");
        if (!word)
        {
                printf("Impossible de créer word.txt\n");
                return 1;
        }

        int s = 0;
        while (1)
        {
                int c = 0;
                int ligne_existante = 0;

                while (1)
                {
                        char fname[64];
                        sprintf(fname, "word_rot_r%02d_c%02d.bmp", s, c);

                        int w, h;
                        if (!SDL_LoadBMP(fname)) break;

                        double* img = load_image(fname, &w, &h);
                        if (!img) break;

                        char letter = predict_letter(img);
                        free(img);

                        printf("%s -> %c\n", fname, letter);
                        fprintf(word, "%c", letter);

                        c++;
                        ligne_existante = 1;
                }

                if (!ligne_existante) break;

                fprintf(word, "\n");
                s++;
        }
        fclose(word);

        SDL_Quit();
        return 0;
}

