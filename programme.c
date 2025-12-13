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

        	double* dataset[312];
        	int labels[312];
        	int w, h;

        	for (int i = 1; i <= 312; i++)
        	{
            		char filename[128];
            		sprintf(filename, "ref/ref_%i.bmp", i);

            		dataset[i - 1] = load_image(filename, &w, &h);
            		if (!dataset[i - 1])
            		{
                		printf("Impossible de charger %s\n", filename);
                		continue;
            		}
	
            		labels[i - 1] = (i - 1) / 12; 
        	}

        	const int EPOCHS = 300;  

        	for (int epoch = 0; epoch < EPOCHS; epoch++)
        	{
            		for (int i = 0; i < 312; i++)
            		{
                		if (dataset[i])
                    		train_sample(dataset[i], labels[i]);
            		}

            		if (epoch % 10 == 0)
                		printf("Époque %d / %d terminée.\n", epoch, EPOCHS);
        	}

        	save_model("model.bin");
        	printf("Entraînement terminé. Modèle sauvegardé.\n");

        	for (int i = 0; i < 234; i++)
            		free(dataset[i]);
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

            		SDL_Surface* check = SDL_LoadBMP(fname);
            		if (!check) break;
            		SDL_FreeSurface(check);

            		int w, h;
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

            		SDL_Surface* check = SDL_LoadBMP(fname);
            		if (!check) break;
            		SDL_FreeSurface(check);

            		int w, h;
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
