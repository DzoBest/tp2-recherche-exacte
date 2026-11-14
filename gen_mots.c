/* Generateur pseudo-aleatoire de mots (un mot par ligne).
   Usage : ./gen_mots <taille_alphabet> <longueur_mot> <nombre_mots> <fichier> [graine] */
#include <stdio.h>
#include <stdlib.h>
#include "alphabet.h"

int main(int argc, char **argv) {
    int sigma, m, nb, i, k;
    FILE *f;
    if (argc < 5) {
        fprintf(stderr, "Usage: %s <alphabet> <longueur> <nombre> <fichier> [graine]\n", argv[0]);
        return 1;
    }
    sigma = atoi(argv[1]); m = atoi(argv[2]); nb = atoi(argv[3]);
    if (sigma < 1 || sigma > ALPHABET_MAX || m < 1 || nb < 1) {
        fprintf(stderr, "Parametres invalides\n");
        return 1;
    }
    srand(argc > 5 ? (unsigned)atoi(argv[5]) : 54321u);
    f = fopen(argv[4], "w");
    if (!f) { perror("fopen"); return 1; }
    for (i = 0; i < nb; i++) {
        for (k = 0; k < m; k++) fputc(ALPHABET[rand() % sigma], f);
        fputc('\n', f);
    }
    fclose(f);
    return 0;
}
