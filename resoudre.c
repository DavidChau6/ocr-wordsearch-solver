#include "resoudre.h"

int resoudre_fichier(const char *fichier_mots, const char *gridfile)
{
    FILE *f = fopen(fichier_mots, "r");
    if (!f) {
        perror("Impossible d'ouvrir le fichier de mots");
        return 1;
    }

    FILE *out = fopen("coord.txt", "w");
    if (!out) {
        perror("Impossible de créer coord.txt");
        fclose(f);
        return 1;
    }

    char ligne[256];

    while (fgets(ligne, sizeof(ligne), f)) {

        
        ligne[strcspn(ligne, "\n")] = 0;

        if (ligne[0] == '\0')
            continue;

        int pipefd[2];
        if (pipe(pipefd) == -1) {
            perror("pipe");
            fclose(f);
            fclose(out);
            return 1;
        }

        pid_t pid = fork();

        if (pid == 0) {



            close(pipefd[0]);           
            dup2(pipefd[1], STDOUT_FILENO);
            close(pipefd[1]);

            execl("./solver", "./solver", gridfile, ligne, (char *)NULL);

            perror("execl a échoué");
            exit(1);
        }
        else if (pid > 0) {
           
            close(pipefd[1]); 

            char buffer[512];
            ssize_t n;

            
            while ((n = read(pipefd[0], buffer, sizeof(buffer) - 1)) > 0) {
                buffer[n] = 0;
                fputs(buffer, out);
            }

            close(pipefd[0]);

            int status;
            waitpid(pid, &status, 0);
        }
        else {
            perror("fork");
            fclose(f);
            fclose(out);
            return 1;
        }
    }

    fclose(f);
    fclose(out);
    return 0;
}


int main(int argc, char *argv[])
{
    if (argc != 3) {
        fprintf(stderr, "Usage : %s fichier_mots grid.txt\n", argv[0]);
        return 1;
    }

    return resoudre_fichier(argv[1], argv[2]);
}

