#include "UI.h"

int traite = 0;

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
	Image(*renderer, "images2/Pret2.png", 50, 50, &page.textmanager[4], &page.button[4],1,1, 0);
	Image(*renderer, "images2/reseau.png", 50, 50 + 160, &page.textmanager[4], &page.button[4],2,1, 0);
	Image(*renderer, "images2/grid.png", 50, 50 + 160 * 2, &page.textmanager[4], &page.button[4],3,1, 0);
	Image(*renderer, "images2/lmots.png", 50, 50 + 160 * 3, &page.textmanager[4], &page.button[4],4,1, 0);
	Image(*renderer, "images2/affichage.png", 50, 50 + 160 * 4, &page.textmanager[4], &page.button[4],5,1, 0);
	Image(*renderer, "images2/save.png", 50, 50 + 160 * 5, &page.textmanager[4], &page.button[4],6,1, 0);
	title(*renderer, "Image initiale :", screen_w / 2 + 50, 150, &page.textmanager[4], 1);
	
	title(*renderer, "PRETRAITEMENT DE L'IMAGE",  screen_w / 2 - 200, 100, &page.textmanager[5], 1);
	Image(*renderer, "images2/Gemini4.png", 0, 0, &page.textmanager[5], &page.button[5], 0, 1, 0);
	Image(*renderer, "images2/Next.png", screen_w / 2, 800, &page.textmanager[5], &page.button[5], 1, 1, 0);
	Image(*renderer, "images2/Next.png", 800, 200, &page.textmanager[5], &page.button[5], 2, 0.6,0);
	pid_t pid = fork();

	if (pid == 0) {
   		char *args[] = {"make", "grid_extract", NULL};
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
    tm->Tlist[tm->count] = tex;
    tm->rects[tm->count] = rect;
    tm->count++;
}


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
	if (mult == 3)
	{
		rect.w = (int)(500);
		rect.h = (int)(500);
	}
	else
    {
		rect.w = (int)(rect.w * mult);
    	rect.h = (int)(rect.h * mult);
	}
	rect.x = x;
	rect.y = y;
    if (index !=0 && button->difficulte[index] == index)
    {
	    SDL_DestroyTexture(textmanager->Tlist[index + nbtitle/*index + (textmanager->count - button->count)*/]);
	    textmanager->Tlist[index + nbtitle/*index + (textmanager->count - button->count)*/] = texture;
            textmanager->rects[index + nbtitle/*index + (textmanager->count - button->count)*/] = rect;
	    return;
    }
    // On remplit directement le bouton et TextManager
    button->rect[index] = rect;
    button->difficulte[index] = index;
    button->count = button->count + 1;
    AddTexture(textmanager, texture, rect);
}


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
	int n_im = 1;
	while(running == 1)
	{	
		running = Event_Handler(renderer, &page, &i, &im, &n_im);
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

int open_file(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        // Le fichier n'existe pas, on le crée
        fclose(file);
	return 1; 
    } else {
        fclose(file);
	return 0;
    }
}

int Event_Handler(SDL_Renderer *renderer, Page* page, int* i, char** currim, int* n_im)
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch(event.type)
		{
			case SDL_QUIT:
				//if (fork() == 0)
				//{
				//	char* arg[] = {"make","extra-clean",NULL};
				//	execvp(arg[0],arg);
				//	perror("execvp a échoué2");
				//	exit(1);
				//}
				return 0;
				break;
			case SDL_MOUSEBUTTONDOWN:
				int mx = event.button.x;
				int my = event.button.y;
				//int index = *i;
				char b = Button_Clicked(&page->button[*i], mx, my);
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
								{
									load = "images2/level_1_image_1.png";    //strcpy(load,"images2/level_1_image_2.png");
									*currim = "images/level_1_image_1.png";
								}
							else	
								{
									load = "images2/level_1_image_2.png";    //strcpy(load,"images2/level_1_image_2.png");
									*currim = "images/level_1_image_2.png";
								}
							*i = 4;
							SDL_RenderClear(renderer);
							Image(renderer, load, 800, 270, &page->textmanager[4], &page->button[4], 7, 1.2,1);
						}
						else if (*i == 2)
						{
							if (b == 2)
							{
								load = "images2/level_2_image_2.png";
								*currim = "images/level_2_image_2.png";
							}
							else
							{
								load = "images2/level_2_image_1.png";
								*currim = "images/level_2_image_1.png";
							}
							*i = 4;
							SDL_RenderClear(renderer);
							Image(renderer, load, 800, 270, &page->textmanager[4], &page->button[4], 7, 1.2,1);
						}
						else if (*i == 3)
						{
							if (b == 1)
								{
									load = "images2/level_3_image_1.png";
									*currim = "images/level_3_image_1.png";
								}
							else
								{
									load = "images2/level_3_image_2.png";
									*currim = "images/level_3_image_2.png";
								}
							*i = 4;
							SDL_RenderClear(renderer);
							Image(renderer, load, 800, 270, &page->textmanager[4], &page->button[4], 7, 1.2,1);
						}
						//printf("%d\n",page->textmanager[4].count);
					}
					else if (*i == 4)
					{
						if (b == 0)
						{
							*i = 0;
							traite = 0;
						}
						//if(b == 1 || b == 2 || b == 3 || b == 4|| b == 5 || b == 6 || b == 7)
						//	printf("bbiizzaaqqrree");
						else if (b == 1)
						{
							*i = 5;
							//A REVOIR -> Fuite de données
							if (*n_im == 1)
							{
								traite = 1;
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
								Image(renderer, "step1_loaded.bmp", 800, 230, &page->textmanager[5], &page->button[5], 2, 3,1);
								*n_im = 2;
							}
						}
						else if (b == 2)
						{
							if(traite == 1) 
							{	
								pid_t pid = fork();
								/*
								if (pid == 0) {
									char *args[] = {"make", "programme", NULL};
									execvp(args[0], args);
									perror("execvp a échoué"); // affichera seulement si execvp échoue
									exit(1);
								} else if (pid > 0) {
									int status;
									waitpid(pid, &status, 0);  // attend que grid_extract finisse vraiment
									if (WIFEXITED(status)) {
										printf("make programme terminé avec code %d\n", WEXITSTATUS(status));
									}
								}
								pid = fork();
								*/
								if (pid == 0) {
    									char *args[] = {"./programme", NULL};
    									execvp(args[0], args);
    									perror("execvp a échoué"); // affichera seulement si execvp échoue
    									exit(1);
								} else if (pid > 0) {
    									int status;
   									waitpid(pid, &status, 0);  // attend que grid_extract finisse vraiment
    									if (WIFEXITED(status)) {
        								printf("programme terminé avec code %d\n", WEXITSTATUS(status));
    									}
								}
							}

						}
						else if (b == 3)
						{
							if (open_file("output.txt") == 0)
								system("xdg-open output.txt");
						}
						else if (b == 4)
						{
							if (open_file("word.txt") == 0)
								system("xdg-open word.txt");
						}
						else if (b == 5)
						{

						}
						else if (b == 6)
						{
							char save[100];
							sprintf(save,"cp %s %s",*currim, "save_image.png");
							system(save);
						}
						
					}
					else if (*i == 5)
					{
						if (b == 0)
						{
							*i = 0;
							*n_im = 1;
						}
						else if (b == 1)
						{
							if (*n_im % 4 == 1)
								Image(renderer, "step1_loaded.bmp", 800, 230, &page->textmanager[5], &page->button[5], 2, 3,1);
							else if (*n_im % 4 == 2)
								Image(renderer, "step2_no_bruit.bmp", 800, 230, &page->textmanager[5], &page->button[5], 2, 3,1);
							else if (*n_im % 4 == 3)
								Image(renderer, "step3_rotated.bmp", 800, 230, &page->textmanager[5], &page->button[5], 2, 3,1);
							else
								Image(renderer, "step4_binary.bmp", 800, 230, &page->textmanager[5], &page->button[5], 2, 3,1);
							*n_im += 1;
						}
					}
					SDL_RenderClear(renderer);
					SDL_RenderPresent(renderer);
					//Image(renderer, load, 140, 350, &page.textmanager[4], &page.button[4], 0, 1);
					RenderCopyFunction(renderer, &page->textmanager[*i]);
					SDL_RenderPresent(renderer);
				}
				break;
		}
	}
	return 1;
}
