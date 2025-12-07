#ifndef RESOUDRE_H
#define RESOUDRE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

/*
 * Exécute ./solver pour chaque mot contenu dans fichier_mots
 * en utilisant gridfile comme fichier grille,
 * capture la sortie de solver et écrit tout dans coord.txt.
 *
 * Retourne 0 si succès, 1 si erreur.
 */
int resoudre_fichier(const char *fichier_mots, const char *gridfile);

#endif

