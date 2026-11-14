/* Générateur pseudo-aléatoire de texte.
   Usage : ./gen_texte <taille_alphabet> <longueur> <fichier_sortie> [graine]

   Le texte est volontairement une séquence continue : aucun séparateur n'est
   ajouté, afin de fournir directement l'entrée aux algorithmes de recherche. */
#include <stdio.h>
#include <stdlib.h>
#include "alphabet.h"

int main(int argc, char **argv) {
    int sigma, i;
    long n;
    FILE *f;
    /* La graine est facultative ; la valeur par défaut rend les expériences
       reproductibles lorsque le programme est relancé. */
    if (argc < 4) {
        fprintf(stderr, "Usage: %s <alphabet> <longueur> <fichier> [graine]\n", argv[0]);
        return 1;
    }
    sigma = atoi(argv[1]);
    n = atol(argv[2]);
    if (sigma < 1 || sigma > ALPHABET_MAX || n < 1) {
        fprintf(stderr, "Parametres invalides (1 <= alphabet <= %d)\n", ALPHABET_MAX);
        return 1;
    }
    srand(argc > 4 ? (unsigned)atoi(argv[4]) : 12345u);
    f = fopen(argv[3], "w");
    if (!f) { perror("fopen"); return 1; }
    /* L'indice est toujours compris entre 0 et sigma - 1. */
    for (i = 0; i < n; i++)
        fputc(ALPHABET[rand() % sigma], f);
    fclose(f);
    return 0;
}
