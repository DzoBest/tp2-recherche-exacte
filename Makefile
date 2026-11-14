CC = gcc
CFLAGS = -O2 -Wall -Wextra

# La cible par défaut construit les deux générateurs et le banc de mesure.
all: gen_texte gen_mots bench

# alphabet.h est une dépendance : modifier l'alphabet invalide les générateurs.
gen_texte: gen_texte.c alphabet.h
	$(CC) $(CFLAGS) -o $@ $<
gen_mots: gen_mots.c alphabet.h
	$(CC) $(CFLAGS) -o $@ $<

# bench regroupe les fonctions de recherche et le programme de mesure.
bench: bench.c algos.c algos.h
	$(CC) $(CFLAGS) -o $@ bench.c algos.c

# Supprime uniquement les artefacts produits par la compilation et l'expérience.
clean:
	rm -rf gen_texte gen_mots bench textes mots resultats.csv
