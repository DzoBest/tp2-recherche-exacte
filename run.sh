#!/bin/bash
# Genere textes et mots, lance toutes les recherches, ecrit resultats.csv
set -e
make -s
mkdir -p textes mots
ALPHABETS="2 4 20 70"
LONGUEURS="4 5 6 7 8 9 10 15 20 30 40 50"
NB_MOTS=100
N=500000

for s in $ALPHABETS; do
    ./gen_texte $s $N textes/texte_$s.txt $((1000 + s))
    for m in $LONGUEURS; do
        ./gen_mots $s $m $NB_MOTS mots/mots_${s}_${m}.txt $((2000 + s * 100 + m))
    done
done

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
