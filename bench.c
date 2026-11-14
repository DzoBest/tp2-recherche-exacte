/* Banc d'essai : lit un texte et un fichier de mots, lance les 11 algorithmes
   sur chaque mot, mesure le temps total par algorithme et verifie que tous
   trouvent le meme nombre d'occurrences.
   Usage : ./bench <texte> <fichier_mots>
   Sortie : une ligne "nom temps_secondes occurrences_totales" par algorithme */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
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

    if (argc < 3) { fprintf(stderr, "Usage: %s <texte> <mots>\n", argv[0]); return 1; }

    /* lecture du texte : n+1 octets, le dernier vaut '\0' (utile a QuickSearch) */
    f = fopen(argv[1], "r");
    if (!f) { perror(argv[1]); return 1; }
    fseek(f, 0, SEEK_END); n = ftell(f); rewind(f);
    y = calloc(n + 1, 1);
    if (fread(y, 1, n, f) != (size_t)n) { fprintf(stderr, "lecture\n"); return 1; }
    fclose(f);

    /* lecture des mots */
    f = fopen(argv[2], "r");
    if (!f) { perror(argv[2]); return 1; }
    while (nb < MAX_MOTS && fgets(ligne, MAX_LEN, f)) {
        ligne[strcspn(ligne, "\n")] = '\0';
        if (ligne[0]) mots[nb++] = strdup(ligne);
    }
    fclose(f);

    long ref = -1;
    for (a = 0; a < NB_ALGOS; a++) {
        long total = 0;
        double t0 = maintenant();
        for (k = 0; k < nb; k++)
            total += algos[a](mots[k], (int)strlen(mots[k]), y, (int)n);
        double t = maintenant() - t0;
        if (ref < 0) ref = total;
        else if (total != ref)
            fprintf(stderr, "ERREUR: %s trouve %ld occurrences (reference %ld)\n",
                    noms[a], total, ref);
        printf("%s %.6f %ld\n", noms[a], t, total);
    }
    return 0;
}
