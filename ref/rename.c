#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

int main() {
    DIR *dir;
    struct dirent *entry;

    int numbers[500];
    int count = 0;

    dir = opendir(".");
    if (!dir) {
        printf("Impossible d'ouvrir le dossier.\n");
        return 1;
    }

    // 1) Récupérer tous les fichiers "ref_xxx.png"
    while ((entry = readdir(dir)) != NULL) {
        int n;
        if (sscanf(entry->d_name, "ref_%d.png", &n) == 1) {
            numbers[count++] = n;
        }
    }
    closedir(dir);

    // 2) Trier du plus grand au plus petit
    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            if (numbers[j] > numbers[i]) {
                int temp = numbers[i];
                numbers[i] = numbers[j];
                numbers[j] = temp;
            }
        }
    }

    // 3) Renommer du plus grand vers le plus petit
    for (int i = 0; i < count; i++) {
        int n = numbers[i];
        int offset = (n - 1) / 5;
        int new_n = n + offset;

        char oldName[256];
        char newName[256];

        sprintf(oldName, "ref_%d.bmp", n);
        sprintf(newName, "ref_%d.bmp", new_n);

        if (strcmp(oldName, newName) != 0) {
            if (rename(oldName, newName) == 0)
                printf("Renommé : %s → %s\n", oldName, newName);
            else
                printf("Erreur en renommant %s\n", oldName);
        }
    }

    return 0;
}

