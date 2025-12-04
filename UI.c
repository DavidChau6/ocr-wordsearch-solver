#include "UI.h"


void initialize(SDL_Window **window, SDL_Renderer **renderer, SDL_Texture **texture)
{
	int Video = SDL_Init(SDL_INIT_VIDEO);
	TTF_Init();
	IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);
	SDL_DisplayMode dm;
	SDL_GetCurrentDisplayMode(0, &dm);
	int screen_w = dm.w;
	int screen_h = dm.h;
	*window = SDL_CreateWindow( "Projection",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,/*800*/screen_w,/*600*/screen_h,0);
	if (window == NULL)
		errx(EXIT_FAILURE,"F");
	//SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
	*renderer = SDL_CreateRenderer(*window, Video ,SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == NULL)
		errx(EXIT_FAILURE,"F");
	*texture = SDL_CreateTexture(*renderer,SDL_PIXELFORMAT_RGBA8888,SDL_TEXTUREACCESS_TARGET,screen_w,screen_h);
	Page page = {0};
	page.textmanager[0] = (TextManager){0};
	page.textmanager[1] = (TextManager){0};
	page.textmanager[2] = (TextManager){0};
	page.textmanager[3] = (TextManager){0};
	page.textmanager[4] = (TextManager){0};
	
	page.textmanager[5] = (TextManager){0};
	page.textmanager[6] = (TextManager){0};
	page.textmanager[7] = (TextManager){0};
	
	page.button[0] = (Button){0};
	page.button[1] = (Button){0};
	page.button[2] = (Button){0};
	page.button[3] = (Button){0};
	page.button[4] = (Button){0};
	
	page.button[5] = (Button){0};
	page.button[6] = (Button){0};
	page.button[7] = (Button){0};
	SDL_SetRenderDrawColor(*renderer, 30, 30, 30, 255); // gris foncé
	SDL_RenderClear(*renderer);
	//title(renderer, "WORD SEARCH SOLVER", screen_w / 2 - 220, 80, &page.textmanager[0], 1);
	//title(renderer, "Moyen", 900, 250, &page.textmanager[0],1);
	//title(renderer, "Facile", 300, 250, &page.textmanager[0],1);
	//title(renderer, "Difficile", 1450, 250, &page.textmanager[0],1);
	Image(*renderer,"images2/Facile3.png", 90, 350, &page.textmanager[0], &page.button[0], 0, 1,0);
	Image(*renderer,"images2/Moyen2.png", 690, 350, &page.textmanager[0], &page.button[0], 1, 1,0);
	Image(*renderer,"images2/Difficile2.png", 1290, 355, &page.textmanager[0], &page.button[0], 2, 1,0);
	title(*renderer, "WORD SEARCH SOLVER", screen_w / 2 - 220, 80, &page.textmanager[0], 1);

	
	Image(*renderer, "images2/Gemini4.png", 0, 0, &page.textmanager[1], &page.button[1], 0, 1, 0);
	title(*renderer, "Choisir une image", screen_w / 2 - 320, 80, &page.textmanager[1], 2);
	Image(*renderer, "images2/level_1_image_1.png", screen_w * 0.12, 280, &page.textmanager[1], &page.button[1], 1, 1, 0);
	Image(*renderer, "images2/level_1_image_2.png", screen_w * 0.55, 280, &page.textmanager[1], &page.button[1], 2, 1, 0);

	
	Image(*renderer, "images2/Gemini4.png", 0, 0, &page.textmanager[2], &page.button[2], 0, 1, 0);
	title(*renderer, "Choisir une image", screen_w / 2 - 320, 80, &page.textmanager[2], 2);
	Image(*renderer, "images2/level_2_image_1.png", screen_w * 0.1, 280, &page.textmanager[2], &page.button[2], 1, 1, 0);
	Image(*renderer, "images2/level_2_image_2.png", screen_w * 0.6, 280, &page.textmanager[2], &page.button[2], 2, 1, 0);
	

	Image(*renderer, "images2/Gemini4.png", 0, 0, &page.textmanager[3], &page.button[3], 0, 1, 0);
	title(*renderer, "Choisir une image", screen_w / 2 - 320, 80, &page.textmanager[3], 2);
	Image(*renderer, "images2/level_3_image_1.png", screen_w * 0.1, 280, &page.textmanager[3], &page.button[3], 1, 1, 0);
	Image(*renderer, "images2/level_3_image_2.png", screen_w * 0.6, 280, &page.textmanager[3], &page.button[3], 2, 1, 0);
	
	Image(*renderer, "images2/Gemini4.png", screen_w - 300, 00, &page.textmanager[4], &page.button[4], 0, 1, 0);
	//printf("%d\n-1",page.textmanager[4].count);
	Image(*renderer, "images2/Pret2.png", 50, 50, &page.textmanager[4], &page.button[4],1,1, 0);
	//printf("%d\n-2",page.textmanager[4].count);
	Image(*renderer, "images2/reseau.png", 50, 50 + 160, &page.textmanager[4], &page.button[4],2,1, 0);
	//printf("%d\n-3",page.textmanager[4].count);
	Image(*renderer, "images2/grid.png", 50, 50 + 160 * 2, &page.textmanager[4], &page.button[4],3,1, 0);
	//printf("%d\n-4",page.textmanager[4].count);
	Image(*renderer, "images2/lmots.png", 50, 50 + 160 * 3, &page.textmanager[4], &page.button[4],4,1, 0);
	//printf("%d\n-5",page.textmanager[4].count);
	Image(*renderer, "images2/affichage.png", 50, 50 + 160 * 4, &page.textmanager[4], &page.button[4],5,1, 0);
	//printf("%d\n-6",page.textmanager[4].count);
	Image(*renderer, "images2/save.png", 50, 50 + 160 * 5, &page.textmanager[4], &page.button[4],6,1, 0);
	//printf("%d\n-7",page.textmanager[4].count);
	title(*renderer, "Image initiale :", screen_w / 2 + 50, 150, &page.textmanager[4], 1);
	//printf("%d\n-8",page.textmanager[4].count);
	
	Image(*renderer, "images2/Gemini4.png", 0, 0, &page.textmanager[5], &page.button[5], 0, 1, 0);
	title(*renderer, "PRETRAITEMENT DE L'IMAGE", screen_w / 2 - 280, 120, &page.textmanager[5], 1);
	pid_t pid = fork();

	if (pid == 0) {
   		char *args[] = {"make", "all", NULL};
    		execvp("make", args);
    		perror("execvp a échoué"); 
    		exit(1);
	}
	principal(*window,*renderer,*texture,page);
}

void RenderCopyFunction(SDL_Renderer *renderer, TextManager* textmanager)
{
	for(int i = 0; i < textmanager->count; i++)
	{
		SDL_RenderCopy(renderer, textmanager->Tlist[i], NULL, &textmanager->rects[i]);
	}
}

void AddTexture(TextManager* tm, SDL_Texture* tex, SDL_Rect rect)
{
    if (!tm) return;  // sécurité
    if (tm->count >= tm->capacity) {
        int new_capacity = (tm->capacity == 0 ? 4 : tm->capacity * 2);
        SDL_Texture** newTlist = realloc(tm->Tlist, new_capacity * sizeof(*tm->Tlist));
        SDL_Rect* newRects = realloc(tm->rects, new_capacity * sizeof(*tm->rects));
        if (!newTlist || !newRects) {
            // erreur mémoire
            return;
        }
        tm->Tlist = newTlist;
        tm->rects = newRects;
        tm->capacity = new_capacity;
    }
    //printf("%d\n",tm->count);
    tm->Tlist[tm->count] = tex;
    tm->rects[tm->count] = rect;
    tm->count++;
}

/*
void AddTexture(TextManager* textmanager, SDL_Texture* tex)
{
    if (textmanager->count >= textmanager->capacity) {
        if (textmanager->capacity == 0) {
            textmanager->capacity = 4;
        } else {
            textmanager->capacity *= 2;
        }
        textmanager->Tlist = realloc(textmanager->Tlist, textmanager->capacity * sizeof(*textmanager->Tlist));
    }
    textmanager->Tlist[textmanager->count++] = tex;
}
*/

void DestroyTextures(TextManager* textmanager)
{
    // Détruire toutes les textures stockées
    for (int i = 0; i < textmanager->count; i++) {
        if (textmanager->Tlist[i]) {
            SDL_DestroyTexture(textmanager->Tlist[i]);
            textmanager->Tlist[i] = NULL; // optionnel mais sûr
        }
    }

    // Libérer le tableau
    free(textmanager->Tlist);
    free(textmanager->rects);
    textmanager->Tlist = NULL;
    textmanager->rects = NULL;

    // Réinitialiser le compteur et la capacité
    textmanager->count = 0;
    textmanager->capacity = 0;
}



void Image(SDL_Renderer *renderer, char* image, int x, int y,
           TextManager* textmanager, Button* button, int index, float mult, int nbtitle)
{
    SDL_Surface* surface = IMG_Load(image);
    if(!surface) {
        printf("Impossible de charger %s\n", image);
        return;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    SDL_Rect rect;
    SDL_QueryTexture(texture, NULL, NULL, &rect.w, &rect.h);
    rect.w = (int)(rect.w * mult);
    rect.h = (int)(rect.h * mult);
    rect.x = x;
    rect.y = y;
   // printf("%d\n",textmanager->count);
    if (index !=0 && button->difficulte[index] == index)
    {
	    //printf("%d\n",textmanager->count);
	    SDL_DestroyTexture(textmanager->Tlist[index + nbtitle/*index + (textmanager->count - button->count)*/]);
	    textmanager->Tlist[index + nbtitle/*index + (textmanager->count - button->count)*/] = texture;
            textmanager->rects[index + nbtitle/*index + (textmanager->count - button->count)*/] = rect;
	    return;
    }
    // On remplit directement le bouton et TextManager
    button->rect[index] = rect;
    button->difficulte[index] = index;
    button->count = button->count + 1;
    //printf("%d\n",index);
    AddTexture(textmanager, texture, rect);
}
/*
SDL_Rect Image(SDL_Renderer *renderer, char* image, int x, int y, TextManager* textmanager, Button* button, int index)
{
	//IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);
	SDL_Surface* surface = IMG_Load(image);
	if(surface == NULL)
		printf("NONZD11234");
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);  // on peut libérer la surface après
	SDL_Rect rect;
	SDL_QueryTexture(texture, NULL, NULL, &rect.w, &rect.h);
	rect.w /= 2;
	rect.h /= 2;
	rect.x = x; (1000 / 3) - rect.w / 2/   // centrer horizontalement
	rect.y = y;
	//button->rect[index] = rect;
	//button->difficulte[index] = index;
	//SDL_RenderCopy(renderer, texture, NULL, &rect);
	AddTexture(textmanager ,texture, rect);
	return rect;
}
*/

void title(SDL_Renderer *renderer, char* phrase, int x, int y, TextManager* textmanager, int mult)
{
	TTF_Font *font = TTF_OpenFont("Roboto_Condensed-BlackItalic.ttf", 48);
	SDL_Color white = {255,255,255,255};
	SDL_Surface *surface = TTF_RenderText_Blended(font, phrase, white);
	SDL_Texture *titre = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);
	SDL_Rect rect;
    	SDL_QueryTexture(titre, NULL, NULL, &rect.w, &rect.h);
   	rect.x = x /*(x - rect.w) / 2*/;   // centrer horizontalement
    	rect.y = y;
	rect.w *= mult;
	rect.h *= mult;
	//SDL_RenderCopy(renderer, titre, NULL, &rect);
	TTF_CloseFont(font);
	AddTexture(textmanager, titre, rect);
        SDL_Delay(16);
}


void principal(SDL_Window *window, SDL_Renderer *renderer, SDL_Texture *texture, Page page)
{
	int i = 0;
	int running = 1;
	RenderCopyFunction(renderer, &page.textmanager[0]);
	char* im = malloc(sizeof(char*));
	SDL_RenderPresent(renderer);
	while(running == 1)
	{	
		running = Event_Handler(renderer, &page, &i, &im);
		//SDL_RenderPresent(renderer);
		//if (i == 4)
			//printf("%d",page.button[4].difficulte[7]);
	}
	SDL_RenderClear(renderer);
	DestroyTextures(&page.textmanager[0]);
	DestroyTextures(&page.textmanager[1]);
	DestroyTextures(&page.textmanager[2]);
	DestroyTextures(&page.textmanager[3]);
	DestroyTextures(&page.textmanager[4]);
	DestroyTextures(&page.textmanager[5]);
	DestroyTextures(&page.textmanager[6]);
	DestroyTextures(&page.textmanager[7]);
}

int Button_Clicked(Button* b, int mx, int my)
{
    for (int i = 0; i < b->count; i++) {
        SDL_Rect r = b->rect[i];
        if (mx >= r.x && mx <= r.x + r.w &&
            my >= r.y && my <= r.y + r.h)
            return b->difficulte[i]; // retourne l'image cliquée
    }
    return -1; // aucun clic
}


int Event_Handler(SDL_Renderer *renderer, Page* page, int* i, char** currim)
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch(event.type)
		{
			case SDL_QUIT:
				if (fork() == 0)
				{
					char* arg[] = {"make","extra-clean",NULL};
					execvp(arg[0],arg);
					perror("execvp a échoué2");
					exit(1);
				}
				return 0;
				break;
			case SDL_MOUSEBUTTONDOWN:
				int mx = event.button.x;
				int my = event.button.y;
				//int index = *i;
				char b = Button_Clicked(&page->button[*i], mx, my);
				//printf("%d - %d\n",b,*i);
				//char* load = "";
				if (b != -1)
				{
					if (*i == 0)
					{
						if (b == 0)
							*i = 1;	//load = "images/level_1_image_2.png";    //strcpy(load,"images/level_1_image_2.png");
						else if (b == 1)
							*i = 2;	//load = "images/level_2_image_2.png";
						else
							*i = 3;	//load = "images/level_3_image_1.png";
					}
					else if (*i == 1 || *i == 2 || *i == 3)
					{
						char* load = "";
						if (b == 0)
							*i = 0;
						else if (*i == 1)
						{
							if (b == 1)
								{load = "images2/level_1_image_1.png";    //strcpy(load,"images2/level_1_image_2.png");
								}
							else	
								{load = "images2/level_1_image_2.png";    //strcpy(load,"images2/level_1_image_2.png");
								}
							*i = 4;
							SDL_RenderClear(renderer);
							Image(renderer, load, 850, 300, &page->textmanager[4], &page->button[4], 7, 1,1);
						}
						else if (*i == 2)
						{
							if (b == 2)
							{load = "images2/level_2_image_2.png";}
							else
							{load = "images2/level_2_image_1.png";}
							*i = 4;
							SDL_RenderClear(renderer);
							Image(renderer, load, 850, 300, &page->textmanager[4], &page->button[4], 7, 1,1);
						}
						else if (*i == 3)
						{
							if (b == 1)
								load = "images2/level_3_image_1.png";
							else
								load = "images2/level_3_image_2.png";
							*i = 4;
							SDL_RenderClear(renderer);
							Image(renderer, load, 850, 300, &page->textmanager[4], &page->button[4], 7, 1,1);
						}
						printf("%d\n",page->textmanager[4].count);
						*currim = load;
					}
					else if (*i == 4)
					{
						if (b == 0)
							*i = 0;
						//if(b == 1 || b == 2 || b == 3 || b == 4|| b == 5 || b == 6 || b == 7)
						//	printf("bbiizzaaqqrree");
						if (b == 1)
						{
							*i = 5;
							//A REVOIR -> Fuite de données
							pid_t pid = fork();
							if (pid == 0) {
    								char *args[] = {"./grid_extract", *currim, NULL};
   								 execvp(args[0], args);
    								perror("execvp a échoué"); // affichera seulement si execvp échoue
    								exit(1);
							} else if (pid > 0) {
    								int status;
    								waitpid(pid, &status, 0);  // attend que grid_extract finisse vraiment
    								if (WIFEXITED(status)) {
        								printf("grid_extract terminé avec code %d\n", WEXITSTATUS(status));
   								}
							}
							Image(renderer, "step1_loaded.bmp", 400, 200, &page->textmanager[5], &page->button[5], 2, 0.6,0);
							Image(renderer, "step2_no_bruit.bmp", 1100, 200, &page->textmanager[5], &page->button[5], 3, 0.6,0);
							Image(renderer, "step3_rotated.bmp", 400, 600, &page->textmanager[5], &page->button[5], 4, 0.6,0);
							Image(renderer, "step4_binary.bmp", 1100, 600, &page->textmanager[5], &page->button[5], 5, 0.6,0);
						}
					}
					else if (*i == 5)
					{
						if (b == 0)
							*i = 0;
					}
					SDL_RenderClear(renderer);
					//SDL_RenderPresent(renderer);
					//Image(renderer, load, 140, 350, &page.textmanager[4], &page.button[4], 0, 1);
					RenderCopyFunction(renderer, &page->textmanager[*i]);
				}
				break;
		}
	}
	return 1;
}
