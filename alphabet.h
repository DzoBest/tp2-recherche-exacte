#ifndef ALPHABET_H
#define ALPHABET_H
/* 70 caracteres imprimables distincts (aucun '\0', aucun espace ni retour
   a la ligne, pour pouvoir stocker un mot par ligne). Un alphabet de taille
   s utilise les s premiers caracteres. */
#define ALPHABET_MAX 70
static const char ALPHABET[ALPHABET_MAX + 1] =
    "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.,;:!?-_";
#endif
