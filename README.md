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
t1 nest pas égal à t2
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

## TD2

### Timers avec callback
Compilez les fichiers sources :
```sh
arm-linux-g++ -Wall -Wextra -Wno-psabi td2a.cpp Chrono.cpp timespec.cpp -o tp2a
```
Sortie :
```sh
Tick 1
Tick 2
Tick 3
Tick 4
Tick 5
Tick 6
Tick 7
Tick 8
Tick 9
Tick 10
Tick 11
Tick 12
Tick 13
Tick 14
Tick 15
```


### Classe Timer
**‍Pour chaque élément de classe Timer, expliquez pourquoi il est public, privé ou protégé.**
1. -timer_t id (privée) : Il est marqué comme privé car il s'agit d'un détail d'implémentation interne à la classe. Seules les méthodes de la classe Timer doivent pouvoir y accéder directement pour éviter toute manipulation incorrecte depuis l'extérieur.

2. +Timer() (public) : Le constructeur est public pour permettre la création d'instances de la classe (ou de ses classes dérivées). Il initialise le timer Posix.

3. +~Timer() (public) : Le destructeur est public pour garantir que les ressources (comme le timer Posix) soient libérées correctement lorsque l'objet est détruit, même depuis un contexte externe.

4. +start(duration: timespec, isPeriodic: bool) & +start_ms(duration_ms: double, isPeriodic: bool) (public): Ces méthodes sont publiques car elles constituent l'interface principale pour démarrer le timer, que ce soit avec une durée en timespec ou en millisecondes. Les utilisateurs de la classe doivent pouvoir les appeler.

5. +stop() (public): Publique pour permettre l'arrêt du timer depuis l'extérieur de la classe.

6. #callback() (protégée) : Cette méthode est déclarée virtuelle pure (abstracte) et protégée car elle doit être implémentée par les classes dérivées, mais n'a pas d'implémentation par défaut dans Timer. Elle n'est pas destinée à être appelée directement par les utilisateurs de la classe, mais uniquement par la classe elle-même (via call_callback) ou ses dérivées. Cela permet de garantir que le callback est invoqué uniquement dans le bon contexte (par exemple, lors de l'expiration du timer).

7. -call_callback(int, siginfo_t*, void*) (privée) Cette méthode est statique pour respecter la signature C exigée par l'API Posix. Elle agit comme un pont entre le handler C et la méthode d'instance callback(). Les utilisateurs n'ont pas besoin d'y accéder directement ; elle est appelée uniquement par le système lors de l'expiration du timer.

### Fonction simple consommant du CPU

### Échantillonage du temps d’exécution d’une fonction

### Classe consommatrice de CPU durant un temps donné


![Courbe des valeurs des temps d’exécution en fonction des valeurs de nLoops](TempsExecution_vs_nLoops_TD2C.png)
