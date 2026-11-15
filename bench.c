/* Banc d'essai : lit un texte et un fichier de mots, lance les 11 algorithmes
   sur chaque mot, mesure le temps total par algorithme et verifie que tous
   trouvent le meme nombre d'occurrences.
   Usage : ./bench <texte> <fichier_mots>
   Sortie : une ligne "nom temps_secondes occurrences_totales" par algorithme */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <limits.h>
#include "algos.h"

#define NB_ALGOS 11
#define MAX_MOTS 1000
#define MAX_LEN 1024

static const char *noms[NB_ALGOS] = {
    "naif_boucle", "naif_boucle_rapide", "naif_boucle_rapide_sentinelle",
    "naif_strncmp", "naif_strncmp_rapide", "naif_strncmp_rapide_sentinelle",
    "MP", "KMP", "BM", "Horspool", "QuickSearch"
};
static algo_t algos[NB_ALGOS] = {
    naif_boucle, naif_boucle_rapide, naif_boucle_rapide_sentinelle,
    naif_strncmp, naif_strncmp_rapide, naif_strncmp_rapide_sentinelle,
    MP, KMP, BM, Horspool, QuickSearch
};

/* CLOCK_MONOTONIC évite qu'une modification de l'heure système perturbe
   la durée mesurée. */
static double maintenant(void) {
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    return t.tv_sec + t.tv_nsec * 1e-9;
}

int main(int argc, char **argv) {
    FILE *f;
    char *y, *mots[MAX_MOTS], ligne[MAX_LEN];
    long n;
    int nb = 0, a, k;

    if (argc < 3) {
        fprintf(stderr, "Usage: %s <texte> <mots>\n", argv[0]);
        return 1;
    }

    /* Le caractère nul final est réservé à QuickSearch, qui lit y[j + m]
       lorsque la fenêtre atteint la fin du texte. */
    f = fopen(argv[1], "r");
    if (!f) { perror(argv[1]); return 1; }
    if (fseek(f, 0, SEEK_END) != 0 || (n = ftell(f)) < 0 ||
        fseek(f, 0, SEEK_SET) != 0 || n > INT_MAX) {
        fprintf(stderr, "Impossible de déterminer la taille du texte\n");
        fclose(f);
        return 1;
    }
    y = calloc(n + 1, 1);
    if (!y) {
        fprintf(stderr, "Allocation impossible pour le texte\n");
        fclose(f);
        return 1;
    }
    if (fread(y, 1, n, f) != (size_t)n) {
        fprintf(stderr, "Erreur de lecture du texte\n");
        free(y);
        fclose(f);
        return 1;
    }
    fclose(f);

    /* Les mots sont conservés en mémoire pour que chaque algorithme mesure
       exactement le même jeu de données. */
    f = fopen(argv[2], "r");
    if (!f) { perror(argv[2]); return 1; }
    while (nb < MAX_MOTS && fgets(ligne, MAX_LEN, f)) {
        ligne[strcspn(ligne, "\n")] = '\0';
        if (ligne[0]) {
            mots[nb] = strdup(ligne);
            if (!mots[nb]) {
                fprintf(stderr, "Allocation impossible pour un mot\n");
                for (k = 0; k < nb; k++) free(mots[k]);
                free(y);
                fclose(f);
                return 1;
            }
            nb++;
        }
    }
    fclose(f);

    /* Le premier résultat sert de référence fonctionnelle pour les suivants.
       Le chronométrage exclut volontairement la lecture et le prétraitement
       des fichiers : il mesure uniquement les recherches. */
    long ref = -1;
    for (a = 0; a < NB_ALGOS; a++) {
        long total = 0;
        double t0 = maintenant();
        for (k = 0; k < nb; k++) {
            int occurrences = algos[a](
                mots[k], (int)strlen(mots[k]), y, (int)n
            );
            if (occurrences < 0) {
                fprintf(stderr, "Erreur mémoire dans %s\n", noms[a]);
                for (k = 0; k < nb; k++) free(mots[k]);
                free(y);
                return 1;
            }
            total += occurrences;
        }
        double t = maintenant() - t0;
        if (ref < 0) ref = total;
        else if (total != ref)
            fprintf(stderr, "ERREUR: %s trouve %ld occurrences (reference %ld)\n",
                    noms[a], total, ref);
        printf("%s %.6f %ld\n", noms[a], t, total);
    }
    for (k = 0; k < nb; k++) free(mots[k]);
    free(y);
    return 0;
}
