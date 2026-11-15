#include <stdio.h>
#include <string.h>
#include "algos.h"

typedef struct {
    const char *nom;
    algo_t fonction;
} algorithme_t;

static const algorithme_t algorithmes[] = {
    {"naif_boucle", naif_boucle},
    {"naif_boucle_rapide", naif_boucle_rapide},
    {"naif_boucle_rapide_sentinelle", naif_boucle_rapide_sentinelle},
    {"naif_strncmp", naif_strncmp},
    {"naif_strncmp_rapide", naif_strncmp_rapide},
    {"naif_strncmp_rapide_sentinelle", naif_strncmp_rapide_sentinelle},
    {"MP", MP},
    {"KMP", KMP},
    {"BM", BM},
    {"Horspool", Horspool},
    {"QuickSearch", QuickSearch}
};

static int verifier(const char *mot, const char *texte, int attendu) {
    size_t nombre = sizeof(algorithmes) / sizeof(algorithmes[0]);
    size_t i;
    for (i = 0; i < nombre; i++) {
        int resultat = algorithmes[i].fonction(
            (char *)mot, (int)strlen(mot),
            (char *)texte, (int)strlen(texte)
        );
        if (resultat != attendu) {
            fprintf(stderr, "%s: \"%s\" dans \"%s\": attendu %d, obtenu %d\n",
                    algorithmes[i].nom, mot, texte, attendu, resultat);
            return 0;
        }
    }
    return 1;
}

int main(void) {
    /* Ces cas couvrent notamment les occurrences chevauchantes et les bornes. */
    if (!verifier("aa", "aaaa", 3)) return 1;
    if (!verifier("abc", "abcabc", 2)) return 1;
    if (!verifier("xyz", "abcabc", 0)) return 1;
    if (!verifier("abcd", "abc", 0)) return 1;
    if (!verifier("a", "aaaa", 4)) return 1;
    printf("Tous les tests des algorithmes sont passes.\n");
    return 0;
}
