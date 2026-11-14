#ifndef ALPHABET_H
#define ALPHABET_H
/* Alphabet commun aux deux générateurs.
   Les caractères sont imprimables et excluent '\0', espace et '\n' afin que
   les mots puissent être stockés sans ambiguïté, un par ligne. */
#define ALPHABET_MAX 70
/* Une expérience de taille sigma utilise uniquement les sigma premiers
   caractères de cette chaîne. Le +1 réserve la terminaison '\0'. */
static const char ALPHABET[ALPHABET_MAX + 1] =
    "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.,;:!?-_";
#endif
