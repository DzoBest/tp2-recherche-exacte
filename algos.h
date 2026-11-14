#ifndef ALGOS_H
#define ALGOS_H

/* Contrat commun à tous les algorithmes :
   x = mot terminé par '\0', m = longueur de x ;
   y = texte terminé par '\0', n = longueur de y.
   La recherche compte aussi les occurrences qui se chevauchent.
   Une valeur négative signale une allocation temporaire impossible. */
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
