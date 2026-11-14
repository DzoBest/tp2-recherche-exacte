/* Générateur pseudo-aléatoire de mots, avec un mot par ligne.
   Usage : ./gen_mots <taille_alphabet> <longueur_mot> <nombre_mots>
                    <fichier> [graine]

   Une graine explicite permet de reproduire exactement un jeu de mots avec
   le même environnement d'exécution. */
#include <stdio.h>
#include <stdlib.h>
#include "alphabet.h"

int main(int argc, char **argv) {
    int sigma, m, nb, i, k;
    FILE *f;
    /* Les quatre premiers arguments utiles sont obligatoires ; la graine est
       facultative et est remplacée par une valeur stable par défaut. */
    if (argc < 5) {
        fprintf(stderr, "Usage: %s <alphabet> <longueur> <nombre> <fichier> [graine]\n", argv[0]);
        return 1;
    }
    sigma = atoi(argv[1]);
    m = atoi(argv[2]);
    nb = atoi(argv[3]);
    if (sigma < 1 || sigma > ALPHABET_MAX || m < 1 || nb < 1) {
        fprintf(stderr, "Parametres invalides\n");
        return 1;
    }
    srand(argc > 5 ? (unsigned)atoi(argv[5]) : 54321u);
    f = fopen(argv[4], "w");
    if (!f) { perror("fopen"); return 1; }

    /* Chaque caractère est tiré dans les sigma premières cases de l'alphabet.
       Le retour à la ligne sépare les mots pour fgets/bench.c. */
    for (i = 0; i < nb; i++) {
        for (k = 0; k < m; k++) fputc(ALPHABET[rand() % sigma], f);
        fputc('\n', f);
    }
    fclose(f);
    return 0;
}
