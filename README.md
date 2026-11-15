# TP d'algorithmique du texte

Ce projet compare onze algorithmes de recherche exacte d'un mot dans un
texte :

- six variantes naïves ;
- Morris-Pratt (`MP`) ;
- Knuth-Morris-Pratt (`KMP`) ;
- Boyer-Moore (`BM`) ;
- Horspool ;
- Quick Search.

Le résultat expérimental est disponible dans
[courbes.png](./courbes.png). Les données brutes se trouvent dans
[resultats.csv](./resultats.csv).

## Reproduire l'expérience

Sous Linux, WSL, Git Bash ou MSYS2 :

```bash
./run.sh
```

Le script :

1. compile les programmes avec le [Makefile](./Makefile) ;
2. génère des textes de longueur `500000` sur les alphabets de tailles
   `2`, `4`, `20` et `70` ;
3. génère, pour chaque alphabet, `100` mots de chacune des longueurs
   `4, 5, 6, 7, 8, 9, 10, 15, 20, 30, 40, 50` ;
4. mesure le temps des onze algorithmes ;
5. écrit les résultats dans [resultats.csv](./resultats.csv) ;
6. produit le graphique [courbes.png](./courbes.png).

Le protocole comporte donc :

```text
4 alphabets × 12 longueurs × 11 algorithmes = 528 mesures
```

Les graines pseudo-aléatoires sont fixées dans le script afin de rendre
l'expérience reproductible.

## Lecture du graphique

![Comparaison des temps d'exécution des algorithmes selon la taille de
l'alphabet et la longueur du mot](./courbes.png)

*Figure 1 — Temps total de recherche pour les 100 mots générés, selon la
taille de l'alphabet et la longueur du mot.*

Chaque sous-graphique correspond à une taille d'alphabet. L'abscisse est la
longueur du mot `m` et l'ordonnée est le temps total de recherche des
100 mots. L'ordonnée est logarithmique : elle permet de comparer des temps
qui peuvent être très différents.

Les courbes en pointillés correspondent aux variantes naïves. Les courbes
continues correspondent à `MP`, `KMP`, `BM`, Horspool et Quick Search.

Il faut comparer les tendances entre les courbes plutôt que les valeurs
absolues d'une machine à l'autre : les temps dépendent du processeur, du
compilateur et de la charge du système.

## Complexités théoriques

On note `n` la longueur du texte et `m` la longueur du mot recherché.

| Algorithme | Prétraitement | Recherche dans le pire cas | Idée principale |
|---|---:|---:|---|
| Naïf avec boucle interne | `O(1)` | `O(nm)` | Compare le mot à chaque position possible |
| Naïf avec boucle rapide | `O(1)` | `O(nm)` | Ignore les positions dont le premier caractère est différent |
| Naïf avec sentinelle | `O(n)` pour la copie | `O(nm)` | Supprime un test de borne dans la boucle rapide |
| Naïf avec `strncmp` | `O(1)` | `O(nm)` | Délègue la comparaison à la bibliothèque |
| Naïf avec `strncmp` rapide | `O(1)` | `O(nm)` | Filtre d'abord sur le premier caractère |
| Naïf avec `strncmp` et sentinelle | `O(n)` pour la copie | `O(nm)` | Combine les deux optimisations précédentes |
| Morris-Pratt | `O(m)` | `O(n + m)` | Utilise les bords du mot après un échec |
| Knuth-Morris-Pratt | `O(m)` | `O(n + m)` | Évite davantage de comparaisons répétées |
| Boyer-Moore | `O(m + alphabet)` | `O(nm)` sans garantie supplémentaire | Décalages par mauvais caractère et bon suffixe |
| Horspool | `O(m + alphabet)` | `O(nm)` | Décalage basé sur le caractère final de la fenêtre |
| Quick Search | `O(m + alphabet)` | `O(nm)` | Décalage basé sur le caractère suivant la fenêtre |

Pour Boyer-Moore, Horspool et Quick Search, le pire cas reste quadratique
pour les versions implémentées ici, mais leur comportement moyen est souvent
très favorable. Ils peuvent sauter plusieurs positions à la fois, ce qui
explique des temps observés bien inférieurs à ceux des algorithmes naïfs.

## Commentaire des courbes

### 1. Variantes naïves

La complexité `O(nm)` explique pourquoi les variantes naïves doivent
effectuer beaucoup de comparaisons lorsque `m` augmente. Leur coût théorique
augmente avec la longueur du mot, même si les mesures peuvent être irrégulières
pour de petites valeurs de `m` à cause des constantes d'implémentation et du
bruit de mesure.

La boucle rapide améliore le cas pratique : elle ne vérifie complètement une
position que lorsque le premier caractère du mot est présent. Son bénéfice
dépend donc de la fréquence de ce caractère dans le texte.

La sentinelle évite un test de limite dans la boucle interne, mais elle impose
une copie de `n` caractères. Elle n'est donc pas automatiquement plus rapide :
le gain d'une comparaison supprimée peut être inférieur au coût de la copie.
De même, `strncmp` bénéficie d'une implémentation de bibliothèque optimisée,
mais cela ne change pas la complexité théorique `O(nm)`.

### 2. Morris-Pratt et KMP

`MP` et `KMP` ont une garantie théorique linéaire en recherche, `O(n + m)`.
Ils réutilisent l'information déjà obtenue sur le mot et ne repartent pas
complètement de zéro après une mauvaise comparaison. KMP améliore encore le
tableau de décalage de MP pour éviter certaines comparaisons inutiles.

Sur les mesures fournies, ces deux algorithmes ne sont pourtant pas toujours
les plus rapides. Une complexité asymptotique favorable ne suffit pas à
dominer les autres algorithmes pour des mots courts : les tableaux de
prétraitement, les accès mémoire et les constantes peuvent alors peser
davantage que le nombre théorique de comparaisons.

### 3. Boyer-Moore, Horspool et Quick Search

Ces algorithmes comparent le mot depuis la droite ou utilisent le caractère
situé après la fenêtre pour déterminer un décalage. Ils peuvent donc éviter
l'examen de nombreuses positions du texte. Cela correspond aux courbes
généralement les plus basses, surtout lorsque l'alphabet est grand et que les
caractères sont plus discriminants.

Quick Search est souvent le meilleur sur les petites et moyennes longueurs
dans les mesures présentes. Horspool devient fréquemment meilleur pour les
grandes longueurs et les alphabets `20` et `70`. Boyer-Moore devient à son
tour compétitif, voire meilleur, dans plusieurs cas avec l'alphabet `2` et
les mots longs.

Ces résultats sont cohérents avec la théorie : la taille de l'alphabet
influence la probabilité qu'un caractère apparaisse dans la fenêtre et donc
la longueur moyenne des sauts. Ils ne constituent toutefois pas une garantie
universelle : un autre texte, une autre machine ou une autre graine peut
modifier le classement.

### 4. Meilleur algorithme observé

Le script sélectionne le temps minimal pour chaque couple `(sigma, m)`. Pour
les données actuellement présentes, on obtient :

| Taille d'alphabet | Algorithmes les plus souvent gagnants |
|---:|---|
| `2` | Quick Search, puis Boyer-Moore pour `m=40` et `m=50` |
| `4` | Quick Search pour toutes les longueurs |
| `20` | Quick Search pour la plupart des longueurs, Horspool pour `m=7`, `30`, `40`, `50` |
| `70` | Variantes naïves à sentinelle pour `m=4` à `m=7`, Quick Search pour `m=8`, puis Horspool à partir de `m=9` |

Les variantes naïves peuvent donc gagner ponctuellement pour les mots très
courts : leur prétraitement est presque nul et le texte est rapidement
éliminé lorsque le premier caractère ne correspond pas. Cela ne contredit
pas leur complexité asymptotique moins favorable.

## Conclusion

Les courbes confirment les tendances attendues :

1. les algorithmes naïfs ont une complexité quadratique dans le pire cas ;
2. MP et KMP garantissent une recherche en `O(n + m)` ;
3. Boyer-Moore, Horspool et Quick Search profitent de grands sauts en pratique ;
4. le meilleur algorithme dépend de `m`, de `sigma` et de la distribution des
   caractères ;
5. la complexité théorique décrit le comportement lorsque les tailles
   augmentent, mais ne prédit pas à elle seule le meilleur temps sur de
   petites entrées.
