#!/bin/bash
# Reproduit l'expérience complète du TP :
# génération des données, mesures, puis production du graphique.
set -e

# Une erreur de compilation ou d'exécution interrompt l'expérience pour éviter
# de produire un CSV incomplet présenté comme un résultat valide.
make -s
mkdir -p textes mots

# Paramètres imposés par l'énoncé du TP.
ALPHABETS="2 4 20 70"
LONGUEURS="4 5 6 7 8 9 10 15 20 30 40 50"
NB_MOTS=100
N=500000

for s in $ALPHABETS; do
    # Le texte est partagé par toutes les longueurs de mots d'un même alphabet.
    ./gen_texte $s $N textes/texte_$s.txt $((1000 + s))
    for m in $LONGUEURS; do
        ./gen_mots $s $m $NB_MOTS mots/mots_${s}_${m}.txt $((2000 + s * 100 + m))
    done
done

# Une ligne du CSV correspond à un couple (alphabet, longueur, algorithme).
echo "alphabet,m,algo,temps,occurrences" > resultats.csv
for s in $ALPHABETS; do
    for m in $LONGUEURS; do
        echo "alphabet=$s m=$m" >&2
        ./bench textes/texte_$s.txt mots/mots_${s}_${m}.txt | \
            while read algo t occ; do echo "$s,$m,$algo,$t,$occ"; done >> resultats.csv
    done
done
echo "Termine : resultats.csv" >&2
python3 plot.py
