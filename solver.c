#include "solver.h"


int main(int argc, char *argv[])
{
	solver(argv[1],argv[2]);
	return 0;
}

void solver(char *fichier,char *mot) //J'ai tous mis en int parceque les vérifications étaient chiante en size_t vu que les size_t peuvent pas être négatifs
{
	FILE *f = fopen(fichier,"r");
	if (f == NULL)
		printf("Not Found");
	else
	{
		int size_w = 0;
		while(*(mot + size_w) != 0)
		{
			if (*(mot + size_w) >= 'a' && *(mot + size_w) <= 'z')
				*(mot + size_w) -= 32;
		       size_w += 1;	
		}
		size_w--;
		char *array[100];
		char ligne[100];
		int height = 0;
		int width = 0;
		while ((fgets(ligne,sizeof(ligne),f)) != NULL) 
		{
			array[height] = strdup(ligne);
			height++;
		}
		fclose(f);
		if (array[0] != NULL)
		{
			width = strlen(array[0]);
			int i = 0;
			int j = 0;
			int l = 0;
			while(i < height)
			{
				j = 0;
				while(j < width)
				{
					if (array[i][j] == *(mot + l))
					{
						int save_i = i;
						int save_j = j;
						l++;
						if (*(mot + l) == 0)
						{
							printf("(%d,%d)(%d,%d)",save_j,save_i,j,i);
							//printf("(%zu,%zu)(%zu,%zu)",i,j,i,j);
							return;
						}
						if (j - size_w >= 0 && array[i][j-1] == *(mot + l))
						{
							l++;
							j -= 2;
							while (/*j >= 0 &&*/ *(mot + l)  != 0 && array[i][j] == *(mot + l))
							{
								j--;
								l++;
							}
							if (*(mot + l) == 0)
							{
								l--;
								printf("(%d,%d)(%d,%d)",save_j,save_i,save_j - l,i);
								//printf("(%zu,%zu)(%zu,%zu)",save_i,save_j,i,j);
								return;
							}
							l = 1;
							j = save_j;		
						}
                                                if (i - size_w >= 0 && j - size_w >= 0 && array[i-1][j-1] == *(mot + l))
                                                {
                                                        l++;
                                                        i -= 2;
							j -= 2;
                                                        while (/*i >= 0 && j >= 0 &&*/ *(mot + l)  != 0 && array[i][j] == *(mot + l))
                                                        {
                                                                i--;
								j--;
                                                                l++;
                                                        }
                                                        if (*(mot + l) == 0)
                                                        {
								l--;
								printf("(%d,%d)(%d,%d)",save_j,save_i,save_j - l,save_i - l);
                                                                //printf("(%zu,%zu)(%zu,%zu)",save_i,save_j,i,j);
                                                                return;
                                                        }
                                                        l = 1;
							i = save_i;
							j = save_j;
                                                }
                                                if (i - size_w >= 0 && array[i-1][j] == *(mot + l))
                                                {
                                                        l++;
                                                        i -= 2;
                                                        while (/*i >= 0 &&*/ *(mot + l)  != 0 && array[i][j] == *(mot + l))
                                                        {
                                                                i--;
                                                                l++;
                                                        }
                                                        if (*(mot + l) == 0)
                                                        {
								l--;
								printf("(%d,%d)(%d,%d)",save_j,save_i,j,save_i - l);
                                                                //printf("(%zu,%zu)(%zu,%zu)",save_i,save_j,i,j);
                                                                return;
                                                        }
                                                        l = 1;
							i = save_i;
                                                }
                                                if (i - size_w >= 0 && j + size_w < width && array[i-1][j+1] == *(mot + l))
                                                {
                                                        l++;
                                                        i -= 2;
							j += 2;
                                                        while (/*i >= 0 && j < width &&*/ *(mot + l)  != 0 && array[i][j] == *(mot + l))
                                                        {
                                                                i--;
								j++;
                                                                l++;
                                                        }
                                                        if (*(mot + l) == 0)
                                                        {
								l--;
								printf("(%d,%d)(%d,%d)",save_j,save_i,save_j + l,save_i - l);
                                                                //printf("(%zu,%zu)(%zu,%zu)",save_i,save_j,i,j);
                                                                return;
                                                        }
                                                        l = 1;
							i = save_i;
							j = save_j;
                                                }
                                                if (j + size_w < width && array[i][j+1] == *(mot + l))
                                                {
                                                        l++;
                                                        j += 2;
                                                        while (/*j < width &&*/ *(mot + l)  != 0 && array[i][j] == *(mot + l))
                                                        {
                                                                j++;
                                                                l++;
                                                        }
                                                        if (*(mot + l) == 0)
                                                        {
								l--;
								printf("(%d,%d)(%d,%d)",save_j,save_i,save_j + l,i);
                                                                //printf("(%zu,%zu)(%zu,%zu)",save_i,save_j,i,j);
                                                                return;
                                                        }
                                                        l = 1;
							j = save_j;
                                                }
                                                if (i + size_w < height && j + size_w < width && array[i+1][j+1] == *(mot + l))
                                                {
                                                        l++;
                                                        i += 2;
							j += 2;
                                                        while (/*i < height && j < width &&*/ *(mot + l)  != 0 && array[i][j] == *(mot + l))
                                                        {
                                                                i++;
								j++;
                                                                l++;
                                                        }
                                                        if (*(mot + l) == 0)
                                                        {
								l--;
								printf("(%d,%d)(%d,%d)",save_j,save_i,save_j + l,save_i + l);
								//printf("(%zu,%zu)(%zu,%zu)",save_i,save_j,i,j);
                                                                return;
                                                        }
                                                        l = 1;
							i = save_i;
							j = save_j;
                                                }
                                                if (i + size_w < height && array[i+1][j] == *(mot + l))
                                                {
                                                        l++;
                                                        i += 2;
                                                        while (/*i < height &&*/ *(mot + l)  != 0 && array[i][j] == *(mot + l))
                                                        {
                                                               	i++; 
                                                                l++;
                                                        }
                                                        if (*(mot + l) == 0)
                                                        {
								l--;
								printf("(%d,%d)(%d,%d)",save_j,save_i,j,save_i + l);
                                                               //printf("(%zu,%zu)(%zu,%zu)",save_i,save_j,i,j);
                                                                return;
                                                        }
                                                        l = 1;
							i = save_i;
                                                }
                                                if (i + size_w < height  && j - size_w >= 0 && array[i+1][j-1] == *(mot + l))
                                                {
                                                        l++;
                                                        i += 2;
							j -= 2;
                                                        while (/*i >= 0 && j >= 0 &&*/ *(mot + l)  != 0 && array[i][j] == *(mot + l))
                                                        {
                                                                i++;
								j--;
                                                                l++;
                                                        }
                                                        if (*(mot + l) == 0)
                                                        {
								l--;
                                                                printf("(%d,%d)(%d,%d)",save_j,save_i,save_j - l,save_i + l);
                                                                return;
                                                        }
                                                        l = 1;
                                                }
						i = save_i;
						j = save_j;
						l = 0;
					}
					j++;
				}
				i++;
			}
		}
		printf("Not Found");
	}
}
