#ifndef ALGOS_H
#define ALGOS_H

/* Tous les algorithmes : x = mot (chaine terminee par '\0'), m = |x|,
   y = texte (chaine terminee par '\0'), n = |y|.
   Retour : nombre d'occurrences (chevauchantes comprises) de x dans y. */
typedef int (*algo_t)(char *x, int m, char *y, int n);

int naif_boucle(char *x, int m, char *y, int n);
int naif_boucle_rapide(char *x, int m, char *y, int n);
int naif_boucle_rapide_sentinelle(char *x, int m, char *y, int n);
int naif_strncmp(char *x, int m, char *y, int n);
int naif_strncmp_rapide(char *x, int m, char *y, int n);
int naif_strncmp_rapide_sentinelle(char *x, int m, char *y, int n);
int MP(char *x, int m, char *y, int n);
int KMP(char *x, int m, char *y, int n);
int BM(char *x, int m, char *y, int n);
int Horspool(char *x, int m, char *y, int n);
int QuickSearch(char *x, int m, char *y, int n);

#endif
