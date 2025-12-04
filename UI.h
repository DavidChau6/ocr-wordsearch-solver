#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stddef.h> 
#include <err.h> 
#include <unistd.h>
#include <sys/wait.h>

//int Event_Handler(SDL_Renderer *renderer);
//void initialize(SDL_Window *window, SDL_Renderer *renderer, SDL_Texture *texture);
//int Event_Handler(SDL_Renderer *renderer, Page page, int i);

typedef struct {
    SDL_Texture** Tlist;
    SDL_Rect* rects;
    int count;
    int capacity;
} TextManager;

typedef struct {
	int difficulte[8];
	SDL_Rect rect[8];
       	int count;		// position + taille
	} Button;

typedef struct {
	Button button[10];
	TextManager textmanager[10];
} Page;

void initialize(SDL_Window **window, SDL_Renderer **renderer, SDL_Texture **texture);
int Event_Handler(SDL_Renderer *renderer, Page* page, int* i, char** im);
void title(SDL_Renderer *renderer, char* phrase, int x, int y, TextManager* textmanager, int mult);
void principal(SDL_Window *window, SDL_Renderer *renderer, SDL_Texture *texture, Page page);
void Image(SDL_Renderer* renderer, char* image, int x, int y, TextManager* textmanager, Button* button, int index, float mult, int nbtitle);