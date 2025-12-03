#include "UI.h"
#include <stddef.h>
#include <SDL2/SDL.h>
#include <err.h>

int main() {
	SDL_Window *window = NULL;
	SDL_Renderer *renderer = NULL;
	SDL_Texture *texture = NULL;
	initialize(&window, &renderer, &texture);
}
