#include <stdlib.h>
#include <string.h>
#include "algos.h"

#define ASIZE 256
#define MAX(a, b) ((a) > (b) ? (a) : (b))

/* ------------------------------------------------------------------ */
/*  Sentinelle : copie de y dans un tampon de n+1 octets dont le       */
/*  dernier octet vaut x[0]. La boucle rapide s'arrete donc toujours,  */
/*  sans avoir a tester j < n.                                         */
/* ------------------------------------------------------------------ */
static char *copie_sentinelle(char *x, char *y, int n) {
    char *buf = malloc(n + 1);
    if (!buf) return NULL;
    memcpy(buf, y, n);
    buf[n] = x[0];
    return buf;
}

/* ------------------------------------------------------------------ */
/*  ALGORITHMES NAIFS AVEC BOUCLE INTERNE                              */
/* ------------------------------------------------------------------ */

/* sans boucle rapide, sans sentinelle */
int naif_boucle(char *x, int m, char *y, int n) {
    int i, j, count = 0;
    if (m > n || m == 0) return 0;
    for (j = 0; j <= n - m; j++) {
        for (i = 0; i < m && x[i] == y[i + j]; i++) ;
        if (i == m) count++;
    }
    return count;
}

/* avec boucle rapide (on saute jusqu'a la prochaine occurrence de x[0]),
   sans sentinelle : il faut tester j <= n-m dans la boucle rapide */
int naif_boucle_rapide(char *x, int m, char *y, int n) {
    int i, j = 0, count = 0;
    if (m > n || m == 0) return 0;
    while (j <= n - m) {
        while (j <= n - m && y[j] != x[0]) j++;
        if (j > n - m) break;
        for (i = 1; i < m && x[i] == y[i + j]; i++) ;
        if (i == m) count++;
        j++;
    }
    return count;
}

/* avec boucle rapide et sentinelle : plus de test de borne dans la boucle rapide */
int naif_boucle_rapide_sentinelle(char *x, int m, char *y, int n) {
    int i, j = 0, count = 0;
    char *t;
    if (m > n || m == 0) return 0;
    t = copie_sentinelle(x, y, n);
    if (!t) return -1;
    for (;;) {
        while (t[j] != x[0]) j++;          /* s'arrete au plus tard en j = n */
        if (j > n - m) break;
        for (i = 1; i < m && x[i] == t[i + j]; i++) ;
        if (i == m) count++;
        j++;
    }
    free(t);
    return count;
}

/* ------------------------------------------------------------------ */
/*  ALGORITHMES NAIFS AVEC strncmp                                     */
/* ------------------------------------------------------------------ */

int naif_strncmp(char *x, int m, char *y, int n) {
    int j, count = 0;
    if (m > n || m == 0) return 0;
    for (j = 0; j <= n - m; j++)
        if (strncmp(x, y + j, m) == 0) count++;
    return count;
}

int naif_strncmp_rapide(char *x, int m, char *y, int n) {
    int j = 0, count = 0;
    if (m > n || m == 0) return 0;
    while (j <= n - m) {
        while (j <= n - m && y[j] != x[0]) j++;
        if (j > n - m) break;
        if (strncmp(x, y + j, m) == 0) count++;
        j++;
    }
    return count;
}

int naif_strncmp_rapide_sentinelle(char *x, int m, char *y, int n) {
    int j = 0, count = 0;
    char *t;
    if (m > n || m == 0) return 0;
    t = copie_sentinelle(x, y, n);
    if (!t) return -1;
    for (;;) {
        while (t[j] != x[0]) j++;
        if (j > n - m) break;
        if (strncmp(x, t + j, m) == 0) count++;
        j++;
    }
    free(t);
    return count;
}

/* ------------------------------------------------------------------ */
/*  MORRIS-PRATT                                                       */
/* ------------------------------------------------------------------ */
static void preMP(char *x, int m, int mpNext[]) {
    int i = 0, j = mpNext[0] = -1;
    while (i < m) {
        while (j > -1 && x[i] != x[j]) j = mpNext[j];
        mpNext[++i] = ++j;
    }
}

int MP(char *x, int m, char *y, int n) {
    int i = 0, j = 0, count = 0;
    int *mpNext;
    if (m > n || m == 0) return 0;
    mpNext = malloc((m + 1) * sizeof(int));
    if (!mpNext) return -1;
    preMP(x, m, mpNext);
    while (j < n) {
        while (i > -1 && x[i] != y[j]) i = mpNext[i];
        i++; j++;
        if (i >= m) { count++; i = mpNext[i]; }
    }
    free(mpNext);
    return count;
}

/* ------------------------------------------------------------------ */
/*  KNUTH-MORRIS-PRATT (bord strict : on evite de retester le meme     */
/*  caractere apres l'echec)                                           */
/* ------------------------------------------------------------------ */
static void preKMP(char *x, int m, int kmpNext[]) {
    int i = 0, j = kmpNext[0] = -1;
    while (i < m) {
        while (j > -1 && x[i] != x[j]) j = kmpNext[j];
        i++; j++;
        if (i < m && x[i] == x[j]) kmpNext[i] = kmpNext[j];
        else kmpNext[i] = j;
    }
}

int KMP(char *x, int m, char *y, int n) {
    int i = 0, j = 0, count = 0;
    int *kmpNext;
    if (m > n || m == 0) return 0;
    kmpNext = malloc((m + 1) * sizeof(int)); 
    if (!kmpNext) return -1;
    preKMP(x, m, kmpNext);
    while (j < n) {
        while (i > -1 && x[i] != y[j]) i = kmpNext[i];
        i++; j++;
        if (i >= m) { count++; i = kmpNext[i]; }
    }
    free(kmpNext);
    return count;
}

/* ------------------------------------------------------------------ */
/*  BOYER-MOORE (mauvais caractere + bon suffixe)                      */
/* ------------------------------------------------------------------ */
static void preBmBc(char *x, int m, int bmBc[]) {
    int i;
    for (i = 0; i < ASIZE; ++i) bmBc[i] = m;
    for (i = 0; i < m - 1; ++i) bmBc[(unsigned char)x[i]] = m - i - 1;
}

static void suffixes(char *x, int m, int *suff) {
    int f = 0, g, i;
    suff[m - 1] = m;
    g = m - 1;
    for (i = m - 2; i >= 0; --i) {
        if (i > g && suff[i + m - 1 - f] < i - g)
            suff[i] = suff[i + m - 1 - f];
        else {
            if (i < g) g = i;
            f = i;
            while (g >= 0 && x[g] == x[g + m - 1 - f]) --g;
            suff[i] = f - g;
        }
    }
}

static void preBmGs(char *x, int m, int bmGs[]) {
    int i, j, *suff = malloc(m * sizeof(int));
    suffixes(x, m, suff);
    for (i = 0; i < m; ++i) bmGs[i] = m;
    j = 0;
    for (i = m - 1; i >= -1; --i)
        if (i == -1 || suff[i] == i + 1)
            for (; j < m - 1 - i; ++j)
                if (bmGs[j] == m) bmGs[j] = m - 1 - i;
    for (i = 0; i <= m - 2; ++i)
        bmGs[m - 1 - suff[i]] = m - 1 - i;
    free(suff);
}

int BM(char *x, int m, char *y, int n) {
    int i, j = 0, count = 0;
    int *bmGs, bmBc[ASIZE];
    if (m > n || m == 0) return 0;
    bmGs = malloc(m * sizeof(int));
    if (!bmGs) return -1;
    preBmGs(x, m, bmGs);
    preBmBc(x, m, bmBc);
    while (j <= n - m) {
        for (i = m - 1; i >= 0 && x[i] == y[i + j]; --i) ;
        if (i < 0) { count++; j += bmGs[0]; }
        else j += MAX(bmGs[i], bmBc[(unsigned char)y[i + j]] - m + 1 + i);
    }
    free(bmGs);
    return count;
}

/* ------------------------------------------------------------------ */
/*  HORSPOOL : decalage par le caractere du texte aligne avec x[m-1]   */
/* ------------------------------------------------------------------ */
int Horspool(char *x, int m, char *y, int n) {
    int j = 0, count = 0, bmBc[ASIZE];
    unsigned char c;
    if (m > n || m == 0) return 0;
    preBmBc(x, m, bmBc);
    while (j <= n - m) {
        c = (unsigned char)y[j + m - 1];
        if (c == (unsigned char)x[m - 1] && memcmp(x, y + j, m - 1) == 0)
            count++;
        j += bmBc[c];
    }
    return count;
}

/* ------------------------------------------------------------------ */
/*  QUICK SEARCH : decalage par le caractere situe juste apres la      */
/*  fenetre (y[j+m]; y[n] = '\0' est valide car y est une chaine C)    */
/* ------------------------------------------------------------------ */
static void preQsBc(char *x, int m, int qsBc[]) {
    int i;
    for (i = 0; i < ASIZE; ++i) qsBc[i] = m + 1;
    for (i = 0; i < m; ++i) qsBc[(unsigned char)x[i]] = m - i;
}

int QuickSearch(char *x, int m, char *y, int n) {
    int j = 0, count = 0, qsBc[ASIZE];
    if (m > n || m == 0) return 0;
    preQsBc(x, m, qsBc);
    while (j <= n - m) {
        if (memcmp(x, y + j, m) == 0) count++;
        j += qsBc[(unsigned char)y[j + m]];
    }
    return count;
}
