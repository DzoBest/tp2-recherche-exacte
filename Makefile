CC = gcc
CFLAGS = -O2 -Wall -Wextra
all: gen_texte gen_mots bench
gen_texte: gen_texte.c alphabet.h
	$(CC) $(CFLAGS) -o $@ $<
gen_mots: gen_mots.c alphabet.h
	$(CC) $(CFLAGS) -o $@ $<
bench: bench.c algos.c algos.h
	$(CC) $(CFLAGS) -o $@ bench.c algos.c
clean:
	rm -rf gen_texte gen_mots bench textes mots resultats.csv
