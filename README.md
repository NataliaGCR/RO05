# CSC_5RO05_TA - GALLEGO Natalia

## TD1

### Gestion simplifiée du temps Posix

Compilez les fichiers sources :
```sh
arm-linux-g++ -Wall -Wextra -Wno-psabi td1a.cpp timespec.cpp -o tp1a
```
Sortie :
```sh
Timespec en millisecondes: 2500 ms
Millisecondes à Timespec: 2 s, 500000000 nsec
Temps actuelle: 534203 s, 322701373 nsec
Négatif de 2 s, 500000000 nsec est: -3 s, 500000000 nsec
Somme de 2 s, 500000000 nsec y 1 s, 700000000 nsec: 4 s, 200000000 nsec
Reste de 2 s, 500000000 nsec y 1 s, 700000000 nsec: 0 s, 800000000 nsec
En attente 1 secondes(s)...
Somme (t1 + t2): 8 s, 700000000 nsec
Reste (t1 - t2): 1 s, 700000000 nsec
Negaf de t1: - -6 s, 800000000 nsec
t1 += t2: 8 s, 700000000 nsec
t1 -= t2: 5 s, 200000000 nsec
t1 n'est pas égal à t2
t1 est différent de t2
t1 est supérieur à t2
```


### Classe Chrono

Compilez les fichiers sources :
```sh
arm-linux-g++ -Wall -Wextra -Wno-psabi td1b.cpp Chrono.cpp timespec.cpp -o tp1b
```
Sortie :
```sh
Démarrage du chronomètre...
Temps écoulé après 2 secondes : 2000.79 ms
Chronomètre arrêté.
Temps écoulé après arrêt : 2001.18 ms
```

## TD1

### Timers avec callback

### Classe Timer

### Fonction simple consommant du CPU

### Échantillonage du temps d’exécution d’une fonction

### Classe consommatrice de CPU durant un temps donné


![Courbe des valeurs des temps d’exécution en fonction des valeurs de nLoops](TempsExecution_vs_nLoops_TD2C.png)
