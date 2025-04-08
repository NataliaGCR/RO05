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

Compilez les fichiers sources :
```sh
arm-linux-g++ -Wall -Wextra -Wno-psabi td2b.cpp Timer.cpp Chrono.cpp timespec.cpp CountDown.cpp -o tp2b
```
Sortie :
```sh
10
9
8
7
6
5
4
3
2
1
0
```

### Fonction simple consommant du CPU
‍**Justifiez quelle méthodes de Looper peuvent être déclarée const**
1. double getSample() const : Cette méthode renvoie simplement la valeur actuelle de iLoop sans modifier aucun membre de la classe. 
2. double stopLoop() : Pas const -> Cette méthode modifie l'attribut doStop (en le passant à true), donc elle ne peut pas être const.
3. double runLoop(double nLoops) : Pas const -> Cette méthode modifie à la fois iLoop (incrémentation) et doStop (réinitialisation), donc elle ne peut pas être const.

**Comment doit-on qualifier les variables doStop et iLoop si on veut être certain que la boucle d’incrémentation s’exécutera comme attendu?**

Pour garantir que la boucle d'incrémentation dans Looper::runLoop() s'exécute comme attendu sans optimisations indésirables du compilateur et avec une synchronisation thread-safe (si le code est utilisé en contexte multithread), les variables doStop et iLoop doivent être qualifiées comme suit :

1. std::atomic<bool> doStop;
    - Atomicité : garantit que les opérations de lecture/écriture sur doStop sont indivisibles (évite les corruptions de données en multithread).
    -  Visibilité : Les modifications de doStop sont immédiatement visibles par tous les threads (pas de caching local).
    -  Évite les optimisations agressives : Le compilateur ne supprimera pas les accès à doStop (contrairement à un bool standard).

2. std::atomic<double> iLoop;
   - Atomicité : Nécessaire si iLoop est accédé/modifié par plusieurs threads.
   -  Précision :assure des opérations thread-safe sur un flottant.

Compilez les fichiers sources :
```sh
arm-linux-g++ -Wall -Wextra Looper.cpp td2c.cpp Chrono.cpp  timespec.cpp -o tp2c
```
Sortie :
```sh
Début des tests de performance...
----------------------------------------
     nLoops | Valeur finale | Temps dexécution
----------------------------------------
nLoops:          100 | Valeur finale:          100 | Temps: 0.000023 s
nLoops:         1000 | Valeur finale:  1000.000000 | Temps: 0.000200 s
nLoops:        10000 | Valeur finale: 10000.000000 | Temps: 0.001927 s
nLoops:       100000 | Valeur finale: 100000.000000 | Temps: 0.019218 s
nLoops:      1000000 | Valeur finale: 1000000.000000 | Temps: 0.192018 s
nLoops:     10000000 | Valeur finale: 10000000.000000 | Temps: 1.919058 s
nLoops:    100000000 | Valeur finale: 100000000.000000 | Temps: 19.188925 s
nLoops:    500000000 | Valeur finale: 500000000.000000 | Temps: 95.944185 s
nLoops:    750000000 | Valeur finale: 750000000.000000 | Temps: 143.916219 s
nLoops:   1000000000 | Valeur finale: 1000000000.000000 | Temps: 191.888351 s
----------------------------------------
Tests terminés.
```

![Courbe des valeurs des temps d’exécution en fonction des valeurs de nLoops](tp2.png)

### Échantillonage du temps d’exécution d’une fonction
Compilez les fichiers sources :
```sh
arm-linux-g++ -Wall -Wextra -Wno-psabi Looper.cpp td2d.cpp Chrono.cpp Timer.cpp Calibrator.cpp timespec.cpp -o tp2d
```
Sortie :
```sh
Inicializando calibrador constructor...
Estimé a: 200000, b: 1000
Temps demandée : 0.5 secondes -> Boucles estimées : 101000
Durée mesurée : 0.5012 secondes
----------------------------------------
Temps demandée : 0.7 secondes -> Boucles estimées : 141000
Durée mesurée : 0.6985 secondes
----------------------------------------
Temps demandée : 0.9 secondes -> Boucles estimées : 181000
Durée mesurée : 0.9007 secondes
----------------------------------------
Temps demandée : 1.1 secondes -> Boucles estimées : 221000
Durée mesurée : 1.0983 secondes
----------------------------------------
Temps demandée : 1.3 secondes -> Boucles estimées : 261000
Durée mesurée : 1.3026 secondes
----------------------------------------
Temps demandée : 1.5 secondes -> Boucles estimées : 301000
Durée mesurée : 1.5029 secondes
----------------------------------------
Temps demandée : 1.7 secondes -> Boucles estimées : 341000
Durée mesurée : 1.7003 secondes
----------------------------------------
Temps demandée : 1.9 secondes -> Boucles estimées : 381000
Durée mesurée : 1.8997 secondes
----------------------------------------
Temps demandée : 2.1 secondes -> Boucles estimées : 421000
Durée mesurée : 2.0989 secondes
----------------------------------------
Temps demandée : 2.3 secondes -> Boucles estimées : 461000
Durée mesurée : 2.2974 secondes
----------------------------------------
Temps demandée : 2.5 secondes -> Boucles estimées : 501000
Durée mesurée : 2.4982 secondes
----------------------------------------
Temps demandée : 2.7 secondes -> Boucles estimées : 541000
Durée mesurée : 2.6991 secondes
----------------------------------------
Temps demandée : 2.9 secondes -> Boucles estimées : 581000
Durée mesurée : 2.9024 secondes
----------------------------------------
Temps demandée : 3.1 secondes -> Boucles estimées : 621000
Durée mesurée : 3.1007 secondes
----------------------------------------
Temps demandée : 3.3 secondes -> Boucles estimées : 661000
Durée mesurée : 3.2986 secondes
----------------------------------------
Temps demandée : 3.5 secondes -> Boucles estimées : 701000
Durée mesurée : 3.4992 secondes
----------------------------------------
Temps demandée : 3.7 secondes -> Boucles estimées : 741000
Durée mesurée : 3.7021 secondes
----------------------------------------
Temps demandée : 3.9 secondes -> Boucles estimées : 781000
Durée mesurée : 3.9005 secondes
----------------------------------------
Temps demandée : 4.1 secondes -> Boucles estimées : 821000
Durée mesurée : 4.0988 secondes
----------------------------------------
Temps demandée : 4.3 secondes -> Boucles estimées : 861000
Durée mesurée : 4.3001 secondes
----------------------------------------
Temps demandée : 4.5 secondes -> Boucles estimées : 4.715e+06  
Durée mesurée : 4.497 secondes  
----------------------------------------  
Temps demandée : 4.7 secondes -> Boucles estimées : 4.909e+06  
Durée mesurée : 4.689 secondes  
----------------------------------------  
Temps demandée : 4.9 secondes -> Boucles estimées : 5.104e+06  
Durée mesurée : 4.894 secondes  
----------------------------------------  
Temps demandée : 5.1 secondes -> Boucles estimées : 5.299e+06  
Durée mesurée : 5.092 secondes  
----------------------------------------  
Temps demandée : 5.3 secondes -> Boucles estimées : 5.494e+06  
Durée mesurée : 5.296 secondes  
----------------------------------------  
Temps demandée : 5.5 secondes -> Boucles estimées : 5.689e+06  
Durée mesurée : 5.497 secondes  
----------------------------------------  
Temps demandée : 5.7 secondes -> Boucles estimées : 5.884e+06  
Durée mesurée : 5.703 secondes  
----------------------------------------  
Temps demandée : 5.9 secondes -> Boucles estimées : 6.079e+06  
Durée mesurée : 5.899 secondes  
----------------------------------------  
Temps demandée : 6.1 secondes -> Boucles estimées : 6.274e+06  
Durée mesurée : 6.104 secondes  
----------------------------------------  
Temps demandée : 6.3 secondes -> Boucles estimées : 6.469e+06  
Durée mesurée : 6.292 secondes  
----------------------------------------  
Temps demandée : 6.5 secondes -> Boucles estimées : 6.664e+06  
Durée mesurée : 6.506 secondes  
----------------------------------------  
Temps demandée : 6.7 secondes -> Boucles estimées : 6.859e+06  
Durée mesurée : 6.693 secondes  
----------------------------------------  
Temps demandée : 6.9 secondes -> Boucles estimées : 7.054e+06  
Durée mesurée : 6.902 secondes  
----------------------------------------  
Temps demandée : 7.1 secondes -> Boucles estimées : 7.249e+06  
Durée mesurée : 7.097 secondes  
----------------------------------------  
Temps demandée : 7.3 secondes -> Boucles estimées : 7.444e+06  
Durée mesurée : 7.301 secondes  
----------------------------------------  
Temps demandée : 7.5 secondes -> Boucles estimées : 7.639e+06  
Durée mesurée : 7.492 secondes  
----------------------------------------  
Temps demandée : 7.7 secondes -> Boucles estimées : 7.834e+06  
Durée mesurée : 7.701 secondes  
----------------------------------------  
Temps demandée : 7.9 secondes -> Boucles estimées : 8.029e+06  
Durée mesurée : 7.898 secondes  
----------------------------------------  
Temps demandée : 8.1 secondes -> Boucles estimées : 8.224e+06  
Durée mesurée : 8.104 secondes  
----------------------------------------  
Temps demandée : 8.3 secondes -> Boucles estimées : 8.419e+06  
Durée mesurée : 8.300 secondes  
----------------------------------------  
Temps demandée : 8.5 secondes -> Boucles estimées : 8.614e+06  
Durée mesurée : 8.497 secondes  
----------------------------------------  
Temps demandée : 8.7 secondes -> Boucles estimées : 8.809e+06  
Durée mesurée : 8.701 secondes  
----------------------------------------  
Temps demandée : 8.9 secondes -> Boucles estimées : 9.004e+06  
Durée mesurée : 8.892 secondes  
----------------------------------------  
Temps demandée : 9.1 secondes -> Boucles estimées : 9.199e+06  
Durée mesurée : 9.106 secondes  
----------------------------------------  
Temps demandée : 9.3 secondes -> Boucles estimées : 9.394e+06  
Durée mesurée : 9.304 secondes  
----------------------------------------  
Temps demandée : 9.5 secondes -> Boucles estimées : 9.589e+06  
Durée mesurée : 9.501 secondes  
----------------------------------------  
Temps demandée : 9.7 secondes -> Boucles estimées : 9.784e+06  
Durée mesurée : 9.698 secondes  
----------------------------------------  
Temps demandée : 9.9 secondes -> Boucles estimées : 9.979e+06  
Durée mesurée : 9.901 secondes  
----------------------------------------
```


### Classe consommatrice de CPU durant un temps donné
Compilez les fichiers sources :
```sh
arm-linux-g++ -Wall -Wextra -Wno-psabi td2e.cpp Looper.cpp CpuLoop.cpp Chrono.cpp Timer.cpp Calibrator.cpp timespec.cpp -o tp2e
```
Sortie :
```sh
Inicializing calibrator constructor...
Estimé a: 5209.29, b: 1.04007e+06
Début des tests de précision...
----------------------------------------
Temps demandé | Temps mesuré | Erreur relative
----------------------------------------
       0.50 s  |      0.700 s |       39.9% <-- ERREUR ÉLEVÉE!
       0.70 s  |      0.899 s |       28.5% <-- ERREUR ÉLEVÉE!
       0.90 s  |      1.099 s |       22.2% <-- ERREUR ÉLEVÉE!
       1.10 s  |      1.299 s |       18.1% <-- ERREUR ÉLEVÉE!
       1.30 s  |      1.499 s |       15.3% <-- ERREUR ÉLEVÉE!
       1.50 s  |      1.699 s |       13.3% <-- ERREUR ÉLEVÉE!
       1.70 s  |      1.899 s |       11.7% <-- ERREUR ÉLEVÉE!
       1.90 s  |      2.099 s |       10.5% <-- ERREUR ÉLEVÉE!
       2.10 s  |      2.300 s |        9.5%
       2.30 s  |      2.499 s |        8.7%
       2.50 s  |      2.699 s |        8.0%
       2.70 s  |      2.899 s |        7.4%
       2.90 s  |      3.099 s |        6.9%
       3.10 s  |      3.298 s |        6.4%
       3.30 s  |      3.499 s |        6.0%
       3.50 s  |      3.698 s |        5.7%
       3.70 s  |      3.898 s |        5.4%
       3.90 s  |      4.098 s |        5.1%
       4.10 s  |      4.298 s |        4.8%
       4.30 s  |      4.498 s |        4.6%
       4.50 s  |      4.698 s |        4.4%
       4.70 s  |      4.898 s |        4.2%
       4.90 s  |      5.098 s |        4.0%
       5.10 s  |      5.298 s |        3.9%
       5.30 s  |      5.498 s |        3.7%
       5.50 s  |      5.698 s |        3.6%
       5.70 s  |      5.898 s |        3.5%
       5.90 s  |      6.098 s |        3.3%
       6.10 s  |      6.297 s |        3.2%
       6.30 s  |      6.497 s |        3.1%
       6.50 s  |      6.697 s |        3.0%
       6.70 s  |      6.897 s |        2.9%
       6.90 s  |      7.097 s |        2.9%
       7.10 s  |      7.297 s |        2.8%
       7.30 s  |      7.497 s |        2.7%
       7.50 s  |      7.697 s |        2.6%
       7.70 s  |      7.897 s |        2.6%
       7.90 s  |      8.097 s |        2.5%
       8.10 s  |      8.297 s |        2.4%
       8.30 s  |      8.497 s |        2.4%
       8.50 s  |      8.696 s |        2.3%
       8.70 s  |      8.896 s |        2.3%
       8.90 s  |      9.096 s |        2.2%
       9.10 s  |      9.296 s |        2.2%
       9.30 s  |      9.496 s |        2.1%
       9.50 s  |      9.696 s |        2.1%
       9.70 s  |      9.897 s |        2.0%
       9.90 s  |     10.096 s |        2.0%
----------------------------------------
Tests terminés.

```


## TD3

### Création et utilisation de tâches et de mutex Posix
**Programmez le programme-test présenté dans le slide 13 du cours.**

```sh
arm-linux-g++ -Wall -Wextra -Wno-psabi td3a_a.cpp -o tp3a_a
```
Sortie :
```sh
Début des tests (3 secondes par test)
----------------------------------------
Test #1 | Compteur: 1.73e+07 | Durée: 3.000s | Mops/s: 5.8
Test #2 | Compteur: 1.73e+07 | Durée: 3.000s | Mops/s: 5.8
Test #3 | Compteur: 1.73e+07 | Durée: 3.000s | Mops/s: 5.8
Test #4 | Compteur: 1.73e+07 | Durée: 3.000s | Mops/s: 5.8
Test #5 | Compteur: 1.72e+07 | Durée: 3.000s | Mops/s: 5.7
Test #6 | Compteur: 1.73e+07 | Durée: 3.000s | Mops/s: 5.8
Test #7 | Compteur: 1.73e+07 | Durée: 3.000s | Mops/s: 5.8
Test #8 | Compteur: 1.73e+07 | Durée: 3.000s | Mops/s: 5.8
Test #9 | Compteur: 1.73e+07 | Durée: 3.000s | Mops/s: 5.8
Test #10 | Compteur: 1.73e+07 | Durée: 3.000s | Mops/s: 5.8
----------------------------------------

```


**Programmez le programme-test présenté dans le slide 15 du cours avec 3 tâches.**
Compilez les fichiers sources :
```sh
arm-linux-g++ -Wall -Wextra -Wno-psabi td3a_b.cpp -o tp3a_b
```
Sortie :
```sh
Début des tests avec 3 threads (3 secondes par test)
------------------------------------
Test #1 | Compteur: 2.81e+07 | Durée: 3.000s | Mops/s: 9.4
Test #2 | Compteur: 2.78e+07 | Durée: 3.000s | Mops/s: 9.3
Test #3 | Compteur: 2.81e+07 | Durée: 3.000s | Mops/s: 9.4
Test #4 | Compteur: 2.79e+07 | Durée: 3.000s | Mops/s: 9.3
Test #5 | Compteur: 2.79e+07 | Durée: 3.000s | Mops/s: 9.3
------------------------------------
Tests terminés

```

**Programmez le programme-test présenté dans le slide 16 du cours.**

Compilez les fichiers sources :
```sh
arm-linux-g++ -Wall -Wextra -Wno-psabi td3a_c.cpp -o tp3a_c
```
Sortie :
```sh
Début des tests avec mutex (3 threads, 3 secondes par test)
--------------------------------------------------------
Test #1 en cours (3 secondes)...
Test #1 | Compteur: 5.31e+06 | Durée: 3.000s | Mops/s: 1.8
Test #2 en cours (3 secondes)...
Test #2 | Compteur: 5.31e+06 | Durée: 3.000s | Mops/s: 1.8
Test #3 en cours (3 secondes)...
Test #3 | Compteur: 5.33e+06 | Durée: 3.000s | Mops/s: 1.8
Test #4 en cours (3 secondes)...
Test #4 | Compteur: 5.32e+06 | Durée: 3.000s | Mops/s: 1.8
Test #5 en cours (3 secondes)...
Test #5 | Compteur: 5.32e+06 | Durée: 3.000s | Mops/s: 1.8
--------------------------------------------------------
Tests terminés

```


### Classe Mutex
Compilez les fichiers sources :
```sh
arm-linux-g++ -Wall -Wextra -Wno-psabi td3b.cpp Mutex.cpp TimeoutException.cpp timespec.cpp -o tp3b
```
Sortie :
```sh
Début des tests avec Mutex (3 threads, 3 secondes par test)
--------------------------------------------------------
Test #1 en cours (3 secondes)...
Test #1 | Compteur: 3.51e+06 | Durée: 3.000s | Mops/s: 1.2
Test #2 en cours (3 secondes)...
Test #2 | Compteur: 3.48e+06 | Durée: 3.000s | Mops/s: 1.2
Test #3 en cours (3 secondes)...
Test #3 | Compteur: 3.50e+06 | Durée: 3.000s | Mops/s: 1.2
Test #4 en cours (3 secondes)...
Test #4 | Compteur: 3.51e+06 | Durée: 3.000s | Mops/s: 1.2
Test #5 en cours (3 secondes)...
Test #5 | Compteur: 3.51e+06 | Durée: 3.000s | Mops/s: 1.2
--------------------------------------------------------
Tests terminés

```

### Classe Thread
Compilez les fichiers sources :
```sh
arm-linux-g++ -Wall -Wextra Looper.cpp td2c.cpp Chrono.cpp  timespec.cpp -o tp2c
```
Sortie :
```sh
Début des tests avec Mutex (3 threads, 3 secondes par test)
--------------------------------------------------------
Test #1 en cours (3 secondes)...
Test #1 | Compteur: 3.63e+06 | Durée: 3.000s | Mops/s: 1.2
Test #2 en cours (3 secondes)...
Test #2 | Compteur: 3.63e+06 | Durée: 3.000s | Mops/s: 1.2
Test #3 en cours (3 secondes)...
Test #3 | Compteur: 3.64e+06 | Durée: 3.000s | Mops/s: 1.2
Test #4 en cours (3 secondes)...
Test #4 | Compteur: 3.63e+06 | Durée: 3.000s | Mops/s: 1.2
Test #5 en cours (3 secondes)...
Test #5 | Compteur: 3.63e+06 | Durée: 3.000s | Mops/s: 1.2
--------------------------------------------------------
Tests terminés

```

### Accès concurrent à des données partagées

**Sans paramètre protect**
Compilez les fichiers sources :
```sh
arm-linux-g++ -Wall -Wextra -Wno-psabi  td3d_a.cpp Thread.cpp timespec.cpp Chrono.cpp Counter.cpp Incrementer.cpp -o tp3d_a
Counter counter(false);
```
Sortie :
```sh
Test avec nLoops = 1000 et nTasks = 1
Valeur attendue: 1000, Valeur obtenue: 1000
Erreur: 0%
Temps d'exécution: 1.42266ms
----------------------------------------
Test avec nLoops = 1000 et nTasks = 2
Valeur attendue: 2000, Valeur obtenue: 1996
Erreur: 0.2%
Temps d'exécution: 1.37448ms
----------------------------------------
Test avec nLoops = 1000 et nTasks = 3
Valeur attendue: 3000, Valeur obtenue: 2999
Erreur: 0.0333333%
Temps d'exécution: 1.26245ms
----------------------------------------
Test avec nLoops = 1000 et nTasks = 4
Valeur attendue: 4000, Valeur obtenue: 3515
Erreur: 12.125%
Temps d'exécution: 1.59042ms
----------------------------------------
Test avec nLoops = 1000 et nTasks = 5
Valeur attendue: 5000, Valeur obtenue: 4682
Erreur: 6.36%
Temps d'exécution: 1.77859ms
----------------------------------------
Test avec nLoops = 10000 et nTasks = 1
Valeur attendue: 10000, Valeur obtenue: 10000
Erreur: 0%
Temps d'exécution: 5.29375ms
----------------------------------------
Test avec nLoops = 10000 et nTasks = 2
Valeur attendue: 20000, Valeur obtenue: 19998
Erreur: 0.01%
Temps d'exécution: 5.42708ms
----------------------------------------
Test avec nLoops = 10000 et nTasks = 3
Valeur attendue: 30000, Valeur obtenue: 25658
Erreur: 14.4733%
Temps d'exécution: 5.59906ms
----------------------------------------
Test avec nLoops = 10000 et nTasks = 4
Valeur attendue: 40000, Valeur obtenue: 24770
Erreur: 38.075%
Temps d'exécution: 5.99042ms
----------------------------------------
Test avec nLoops = 10000 et nTasks = 5
Valeur attendue: 50000, Valeur obtenue: 35041
Erreur: 29.918%
Temps d'exécution: 11.1985ms
----------------------------------------
Test avec nLoops = 100000 et nTasks = 1
Valeur attendue: 100000, Valeur obtenue: 100000
Erreur: 0%
Temps d'exécution: 47.0732ms
----------------------------------------
Test avec nLoops = 100000 et nTasks = 2
Valeur attendue: 200000, Valeur obtenue: 199854
Erreur: 0.073%
Temps d'exécution: 49.0901ms
----------------------------------------
Test avec nLoops = 100000 et nTasks = 3
Valeur attendue: 300000, Valeur obtenue: 249431
Erreur: 16.8563%
Temps d'exécution: 51.5536ms
----------------------------------------
Test avec nLoops = 100000 et nTasks = 4
Valeur attendue: 400000, Valeur obtenue: 239581
Erreur: 40.1048%
Temps d'exécution: 51.9282ms
----------------------------------------
Test avec nLoops = 100000 et nTasks = 5
Valeur attendue: 500000, Valeur obtenue: 321313
Erreur: 35.7374%
Temps d'exécution: 82.5723ms
----------------------------------------
Test avec nLoops = 1000000 et nTasks = 1
Valeur attendue: 1e+06, Valeur obtenue: 1e+06
Erreur: 0%
Temps d'exécution: 459.808ms
----------------------------------------
Test avec nLoops = 1000000 et nTasks = 2
Valeur attendue: 2e+06, Valeur obtenue: 1.99883e+06
Erreur: 0.0583%
Temps d'exécution: 484.791ms
----------------------------------------
Test avec nLoops = 1000000 et nTasks = 3
Valeur attendue: 3e+06, Valeur obtenue: 2.45297e+06
Erreur: 18.2344%
Temps d'exécution: 506.922ms
----------------------------------------
Test avec nLoops = 1000000 et nTasks = 4
Valeur attendue: 4e+06, Valeur obtenue: 2.41374e+06
Erreur: 39.6565%
Temps d'exécution: 514.459ms
----------------------------------------
Test avec nLoops = 1000000 et nTasks = 5
Valeur attendue: 5e+06, Valeur obtenue: 2.91452e+06
Erreur: 41.7096%
Temps d'exécution: 672.233ms
----------------------------------------
Test avec nLoops = 10000000 et nTasks = 1
Valeur attendue: 1e+07, Valeur obtenue: 1e+07
Erreur: 0%
Temps d'exécution: 4591.44ms
----------------------------------------
Test avec nLoops = 10000000 et nTasks = 2
Valeur attendue: 2e+07, Valeur obtenue: 1.99886e+07
Erreur: 0.057145%
Temps d'exécution: 4840.9ms
----------------------------------------
Test avec nLoops = 10000000 et nTasks = 3
Valeur attendue: 3e+07, Valeur obtenue: 2.45801e+07
Erreur: 18.0663%
Temps d'exécution: 5059.88ms
----------------------------------------
Test avec nLoops = 10000000 et nTasks = 4
Valeur attendue: 4e+07, Valeur obtenue: 2.41272e+07
Erreur: 39.682%
Temps d'exécution: 5137.75ms
----------------------------------------
Test avec nLoops = 10000000 et nTasks = 5
Valeur attendue: 5e+07, Valeur obtenue: 2.9496e+07
Erreur: 41.008%
Temps d'exécution: 6431.8ms
----------------------------------------
Test avec nLoops = 100000000 et nTasks = 1
Valeur attendue: 1e+08, Valeur obtenue: 1e+08
Erreur: 0%
Temps d'exécution: 46077.1ms
----------------------------------------
Test avec nLoops = 100000000 et nTasks = 2
Valeur attendue: 2e+08, Valeur obtenue: 1.99979e+08
Erreur: 0.0103285%
Temps d'exécution: 48173ms
----------------------------------------
Test avec nLoops = 100000000 et nTasks = 3
Valeur attendue: 3e+08, Valeur obtenue: 2.33004e+08
Erreur: 22.3321%
Temps d'exécution: 50492.7ms
----------------------------------------
Test avec nLoops = 100000000 et nTasks = 4
Valeur attendue: 4e+08, Valeur obtenue: 2.41502e+08
Erreur: 39.6246%
Temps d'exécution: 51386.7ms
----------------------------------------
Test avec nLoops = 100000000 et nTasks = 5
Valeur attendue: 5e+08, Valeur obtenue: 2.94438e+08
Erreur: 41.1123%
Temps d'exécution: 64235.4ms
----------------------------------------

```

Décrivez ce que vous constatez: 

Les résultats montrent qu'à mesure que le nombre de tâches (nTasks) augmente, l'erreur dans la valeur obtenue du compteur augmente également, en particulier lorsque le nombre d'itérations (nLoops) est élevé. Au début, avec peu de threads, le compteur atteint des valeurs très proches de celles attendues, avec des erreurs minimales (moins de 1%). Cependant, avec plus de 3 tâches, une divergence croissante entre la valeur attendue et la valeur obtenue est observée, ce qui indique que les accès concurrents ne sont pas parfaitement synchronisés. Cela se reflète également par une augmentation significative du temps d'exécution, surtout pour des valeurs de nLoops très élevées, ce qui suggère que l'implémentation des threads pourrait être confrontée à des blocages ou des attentes dues à un manque de protection adéquate du compteur partagé.

**Avec paramètre protect**
Compilez les fichiers sources :
```sh
arm-linux-g++ -Wall -Wextra -Wno-psabi  td3d_a.cpp Thread.cpp timespec.cpp Chrono.cpp Counter.cpp Incrementer.cpp -o tp3d_a
Counter counter(true);
```
Sortie :
```sh
Test avec nLoops = 1000 et nTasks = 1
Valeur attendue: 1000, Valeur obtenue: 1000
Erreur: 0%
Temps d'exécution: 2.14557ms
----------------------------------------
Test avec nLoops = 1000 et nTasks = 2
Valeur attendue: 2000, Valeur obtenue: 2000
Erreur: 0%
Temps d'exécution: 1.95047ms
----------------------------------------
Test avec nLoops = 1000 et nTasks = 3
Valeur attendue: 3000, Valeur obtenue: 3000
Erreur: 0%
Temps d'exécution: 3.58656ms
----------------------------------------
Test avec nLoops = 1000 et nTasks = 4
Valeur attendue: 4000, Valeur obtenue: 4000
Erreur: 0%
Temps d'exécution: 5.34245ms
----------------------------------------
Test avec nLoops = 1000 et nTasks = 5
Valeur attendue: 5000, Valeur obtenue: 5000
Erreur: 0%
Temps d'exécution: 6.73187ms
----------------------------------------
Test avec nLoops = 10000 et nTasks = 1
Valeur attendue: 10000, Valeur obtenue: 10000
Erreur: 0%
Temps d'exécution: 12.5159ms
----------------------------------------
Test avec nLoops = 10000 et nTasks = 2
Valeur attendue: 20000, Valeur obtenue: 20000
Erreur: 0%
Temps d'exécution: 13.9408ms
----------------------------------------
Test avec nLoops = 10000 et nTasks = 3
Valeur attendue: 30000, Valeur obtenue: 30000
Erreur: 0%
Temps d'exécution: 32.1775ms
----------------------------------------
Test avec nLoops = 10000 et nTasks = 4
Valeur attendue: 40000, Valeur obtenue: 40000
Erreur: 0%
Temps d'exécution: 40.7585ms
----------------------------------------
Test avec nLoops = 10000 et nTasks = 5
Valeur attendue: 50000, Valeur obtenue: 50000
Erreur: 0%
Temps d'exécution: 46.3945ms
----------------------------------------
Test avec nLoops = 100000 et nTasks = 1
Valeur attendue: 100000, Valeur obtenue: 100000
Erreur: 0%
Temps d'exécution: 123.615ms
----------------------------------------
Test avec nLoops = 100000 et nTasks = 2
Valeur attendue: 200000, Valeur obtenue: 200000
Erreur: 0%
Temps d'exécution: 133.234ms
----------------------------------------
Test avec nLoops = 100000 et nTasks = 3
Valeur attendue: 300000, Valeur obtenue: 300000
Erreur: 0%
Temps d'exécution: 310.374ms
----------------------------------------
Test avec nLoops = 100000 et nTasks = 4
Valeur attendue: 400000, Valeur obtenue: 400000
Erreur: 0%
Temps d'exécution: 510.194ms
----------------------------------------
Test avec nLoops = 100000 et nTasks = 5
Valeur attendue: 500000, Valeur obtenue: 500000
Erreur: 0%
Temps d'exécution: 603.106ms
----------------------------------------
Test avec nLoops = 1000000 et nTasks = 1
Valeur attendue: 1e+06, Valeur obtenue: 1e+06
Erreur: 0%
Temps d'exécution: 1230.62ms
----------------------------------------
Test avec nLoops = 1000000 et nTasks = 2
Valeur attendue: 2e+06, Valeur obtenue: 2e+06
Erreur: 0%
Temps d'exécution: 1326.98ms
----------------------------------------
Test avec nLoops = 1000000 et nTasks = 3
Valeur attendue: 3e+06, Valeur obtenue: 3e+06
Erreur: 0%
Temps d'exécution: 3106.03ms
----------------------------------------
Test avec nLoops = 1000000 et nTasks = 4
Valeur attendue: 4e+06, Valeur obtenue: 4e+06
Erreur: 0%
Temps d'exécution: 5104.93ms
----------------------------------------
Test avec nLoops = 1000000 et nTasks = 5
Valeur attendue: 5e+06, Valeur obtenue: 5e+06
Erreur: 0%
Temps d'exécution: 5982.81ms
----------------------------------------
Test avec nLoops = 10000000 et nTasks = 1
Valeur attendue: 1e+07, Valeur obtenue: 1e+07
Erreur: 0%
Temps d'exécution: 12288ms
----------------------------------------
Test avec nLoops = 10000000 et nTasks = 2
Valeur attendue: 2e+07, Valeur obtenue: 2e+07
Erreur: 0%
Temps d'exécution: 13271.4ms
----------------------------------------
Test avec nLoops = 10000000 et nTasks = 3
Valeur attendue: 3e+07, Valeur obtenue: 3e+07
Erreur: 0%
Temps d'exécution: 31146.6ms
----------------------------------------
Test avec nLoops = 10000000 et nTasks = 4
Valeur attendue: 4e+07, Valeur obtenue: 4e+07
Erreur: 0%
Temps d'exécution: 51212.4ms
----------------------------------------
Test avec nLoops = 10000000 et nTasks = 5
Valeur attendue: 5e+07, Valeur obtenue: 5e+07
Erreur: 0%
Temps d'exécution: 60212.8ms
----------------------------------------
Test avec nLoops = 100000000 et nTasks = 1
Valeur attendue: 1e+08, Valeur obtenue: 1e+08
Erreur: 0%
Temps d'exécution: 122844ms
----------------------------------------
Test avec nLoops = 100000000 et nTasks = 2
Valeur attendue: 2e+08, Valeur obtenue: 2e+08
Erreur: 0%
Temps d'exécution: 132715ms
----------------------------------------
Test avec nLoops = 100000000 et nTasks = 3
Valeur attendue: 3e+08, Valeur obtenue: 3e+08
Erreur: 0%
Temps d'exécution: 311496ms
----------------------------------------
Test avec nLoops = 100000000 et nTasks = 4
Valeur attendue: 4e+08, Valeur obtenue: 4e+08
Erreur: 0%
Temps d'exécution: 512355ms
----------------------------------------
Test avec nLoops = 100000000 et nTasks = 5
Valeur attendue: 5e+08, Valeur obtenue: 5e+08
Erreur: 0%
Temps d'exécution: 649559ms
----------------------------------------
```

Expliquez les variations importantes dans les temps d’exécution :
La version **non protégée** (sans mutex) est  plus rapide car les threads peuvent accéder simultanément au compteur sans attendre de verrouillage, mais cela peut entraîner des erreurs dues aux conditions de concurrence, rendant le résultat final incorrect. En revanche, la version **protégée** (avec mutex) garantit l'exactitude du compteur en synchronisant l'accès, mais cela introduit des délais dus aux verrous, ce qui ralentit l'exécution, surtout avec un grand nombre de threads. En somme, la version protégée offre une exécution correcte mais plus lente, tandis que la version non protégée est plus rapide mais risque de donner des résultats incorrects.

**Avec la politique d’ordonnacement fixée à SCHED_RR**
Compilez les fichiers sources :
```sh
arm-linux-g++ -Wall -Wextra -Wno-psabi  td3d_b.cpp Thread.cpp timespec.cpp Chrono.cpp Counter.cpp Incrementer.cpp -o tp3d_b
```
Sortie : 
```sh

Politique d'ordonnancement : SCHED_OTHER (par défaut)
Test avec nLoops = 1000 et nTasks = 1
Valeur attendue: 1000, Valeur obtenue: 1000
Erreur: 0%
Temps d'exécution: 1.34823ms
----------------------------------------
Test avec nLoops = 1000 et nTasks = 2
Valeur attendue: 2000, Valeur obtenue: 1998
Erreur: 0.1%
Temps d'exécution: 1.12854ms
----------------------------------------
Test avec nLoops = 1000 et nTasks = 3
Valeur attendue: 3000, Valeur obtenue: 2906
Erreur: 3.13333%
Temps d'exécution: 1.22135ms
----------------------------------------
Test avec nLoops = 1000 et nTasks = 4
Valeur attendue: 4000, Valeur obtenue: 3575
Erreur: 10.625%
Temps d'exécution: 1.45578ms
----------------------------------------
Test avec nLoops = 1000 et nTasks = 5
Valeur attendue: 5000, Valeur obtenue: 4487
Erreur: 10.26%
Temps d'exécution: 1.70865ms
----------------------------------------
Test avec nLoops = 10000 et nTasks = 1
Valeur attendue: 10000, Valeur obtenue: 10000
Erreur: 0%
Temps d'exécution: 4.94021ms
----------------------------------------
Test avec nLoops = 10000 et nTasks = 2
Valeur attendue: 20000, Valeur obtenue: 19997
Erreur: 0.015%
Temps d'exécution: 5.22625ms
----------------------------------------
Test avec nLoops = 10000 et nTasks = 3
Valeur attendue: 30000, Valeur obtenue: 21817
Erreur: 27.2767%
Temps d'exécution: 5.55719ms
----------------------------------------
Test avec nLoops = 10000 et nTasks = 4
Valeur attendue: 40000, Valeur obtenue: 25599
Erreur: 36.0025%
Temps d'exécution: 5.9426ms
----------------------------------------
Test avec nLoops = 10000 et nTasks = 5
Valeur attendue: 50000, Valeur obtenue: 33748
Erreur: 32.504%
Temps d'exécution: 10.4257ms
----------------------------------------
Test avec nLoops = 100000 et nTasks = 1
Valeur attendue: 100000, Valeur obtenue: 100000
Erreur: 0%
Temps d'exécution: 46.9738ms
----------------------------------------
Test avec nLoops = 100000 et nTasks = 2
Valeur attendue: 200000, Valeur obtenue: 199991
Erreur: 0.0045%
Temps d'exécution: 48.7408ms
----------------------------------------
Test avec nLoops = 100000 et nTasks = 3
Valeur attendue: 300000, Valeur obtenue: 242450
Erreur: 19.1833%
Temps d'exécution: 51.1909ms
----------------------------------------
Test avec nLoops = 100000 et nTasks = 4
Valeur attendue: 400000, Valeur obtenue: 244636
Erreur: 38.841%
Temps d'exécution: 52.0723ms
----------------------------------------
Test avec nLoops = 100000 et nTasks = 5
Valeur attendue: 500000, Valeur obtenue: 343738
Erreur: 31.2524%
Temps d'exécution: 78.9493ms
----------------------------------------
Test avec nLoops = 1000000 et nTasks = 1
Valeur attendue: 1e+06, Valeur obtenue: 1e+06
Erreur: 0%
Temps d'exécution: 459.679ms
----------------------------------------
Test avec nLoops = 1000000 et nTasks = 2
Valeur attendue: 2e+06, Valeur obtenue: 1.99986e+06
Erreur: 0.0072%
Temps d'exécution: 482.439ms
----------------------------------------
Test avec nLoops = 1000000 et nTasks = 3
Valeur attendue: 3e+06, Valeur obtenue: 2.45063e+06
Erreur: 18.3124%
Temps d'exécution: 506.614ms
----------------------------------------
Test avec nLoops = 1000000 et nTasks = 4
Valeur attendue: 4e+06, Valeur obtenue: 2.40154e+06
Erreur: 39.9614%
Temps d'exécution: 514.402ms
----------------------------------------
Test avec nLoops = 1000000 et nTasks = 5
Valeur attendue: 5e+06, Valeur obtenue: 3.01636e+06
Erreur: 39.6727%
Temps d'exécution: 659.817ms
----------------------------------------
Test avec nLoops = 10000000 et nTasks = 1
Valeur attendue: 1e+07, Valeur obtenue: 1e+07
Erreur: 0%
Temps d'exécution: 4609.3ms
----------------------------------------
Test avec nLoops = 10000000 et nTasks = 2
Valeur attendue: 2e+07, Valeur obtenue: 1.99883e+07
Erreur: 0.05874%
Temps d'exécution: 4842.88ms
----------------------------------------
Test avec nLoops = 10000000 et nTasks = 3
Valeur attendue: 3e+07, Valeur obtenue: 2.41462e+07
Erreur: 19.5126%
Temps d'exécution: 5048.62ms
----------------------------------------
Test avec nLoops = 10000000 et nTasks = 4
Valeur attendue: 4e+07, Valeur obtenue: 2.41175e+07
Erreur: 39.7062%
Temps d'exécution: 5139.25ms
----------------------------------------
Test avec nLoops = 10000000 et nTasks = 5
Valeur attendue: 5e+07, Valeur obtenue: 2.97125e+07
Erreur: 40.5749%
Temps d'exécution: 6541.34ms
----------------------------------------
Test avec nLoops = 100000000 et nTasks = 1
Valeur attendue: 1e+08, Valeur obtenue: 1e+08
Erreur: 0%
Temps d'exécution: 45972.3ms
----------------------------------------
Test avec nLoops = 100000000 et nTasks = 2
Valeur attendue: 2e+08, Valeur obtenue: 1.99884e+08
Erreur: 0.057913%
Temps d'exécution: 48425ms
----------------------------------------
Test avec nLoops = 100000000 et nTasks = 3
Valeur attendue: 3e+08, Valeur obtenue: 2.45739e+08
Erreur: 18.0869%
Temps d'exécution: 50607.7ms
----------------------------------------
Test avec nLoops = 100000000 et nTasks = 4
Valeur attendue: 4e+08, Valeur obtenue: 2.41411e+08
Erreur: 39.6472%
Temps d'exécution: 51387.1ms
----------------------------------------
Test avec nLoops = 100000000 et nTasks = 5
Valeur attendue: 5e+08, Valeur obtenue: 2.94586e+08
Erreur: 41.0828%
Temps d'exécution: 64199.7ms
----------------------------------------

```

### Inversion de priorité

Compilez les fichiers sources :
```sh
arm-linux-g++ -Wall -Wextra -Wno-psabi td3e.cpp Thread.cpp timespec.cpp CpuLoop.cpp Chrono.cpp Counter.cpp Incrementer.cpp Calibrator.cpp Mutex.cpp Timer.cpp Looper.cpp -o tp3e
```

Remarque : Cet exercice n'a pas pu être exécuté pendant le cours en raison de certaines erreurs. Une simulation a donc été réalisée sur la manière dont le code se comporterait une fois les problèmes corrigés. Les valeurs ne sont donc pas exactes.


Sortie : 
```sh
=============================
Running scenario WITHOUT priority inheritance
=============================
[C] Starting at 0 ms
[C] Waiting for resource at 2000 ms
[A] Starting at 3000 ms
[A] Waiting for resource at 4000 ms
[B] Starting at 3000 ms
[B] Finished at 4000 ms
[C] Locked resource at 5000 ms
[C] Released resource at 7000 ms
[A] Locked resource at 7000 ms
[A] Released resource at 8000 ms
[A] Finished at 10000 ms
[C] Finished at 11000 ms
-----------------------------
Results:
Task A elapsed time: 7000 ms
Task B elapsed time: 1000 ms
Task C elapsed time: 11000 ms
=============================


-------------------------------------------------------------------------------------------------

=============================
Running scenario WITH priority inheritance
=============================
[C] Starting at 0 ms
[C] Waiting for resource at 2000 ms
[A] Starting at 3000 ms
[A] Waiting for resource at 4000 ms
[B] Starting at 3000 ms
[B] Finished at 4000 ms
[C] Locked resource at 4000 ms
[C] Released resource at 6000 ms
[A] Locked resource at 6000 ms
[A] Released resource at 7000 ms
[A] Finished at 9000 ms
[C] Finished at 10000 ms
-----------------------------
Results:
Task A elapsed time: 6000 ms
Task B elapsed time: 1000 ms
Task C elapsed time: 10000 ms
=============================

```


**Sans héritage de priorité (WITHOUT priority inheritance)**
- Tâche C (basse priorité) commence à t=0 ms, puis tente d’accéder à une ressource partagée à t=2000 ms. Elle attend.
- Tâche A (haute priorité) démarre à t=3000 ms et veut aussi accéder à la ressource à t=4000 ms → mais elle doit attendre que C libère la ressource.
- Tâche B (priorité moyenne) s’exécute entre t=3000 et t=4000 ms sans problème.
La tâche C obtient la ressource à t=5000 ms, la libère à t=7000 ms, ce qui permet à A de continuer.
Résultat : la tâche A est bloquée inutilement pendant longtemps, car B est passée entre-temps, provoquant une inversion de priorité.
Temps de la tâche A : 7000 ms

**Avec héritage de priorité (WITH priority inheritance)**
Le scénario est similaire au départ.
- Quand la tâche A (priorité haute) est bloquée par C (priorité basse), la priorité de C est temporairement élevée.
- Cela permet à C d’accéder plus rapidement à la ressource à t=4000 ms (au lieu de 5000 ms) et de la libérer à t=6000 ms.

A peut alors continuer plus tôt.
Résultat : l’inversion de priorité est évitée, car C hérite temporairement de la priorité d’A et termine plus vite.
Temps de la tâche A : 6000 ms


## TD4

### Création de la classe Monitor et Création d’une classe Sémaphore


Compilez les fichiers sources :
```sh
arm-linux-g++ -Wall -Wextra -Wno-psabi  td4b.cpp Thread.cpp Semaphore.cpp Mutex.cpp Monitor.cpp timespec.cpp TimeoutException.cpp -o tp4b
```


Sortie : 
```sh

Résultats finaux:
Total jetons produits: 3000
Total jetons consommés: 3000
Bilan: 0 jetons non consommés
SUCCÈS: Aucun jeton perdu!
real    0m 1.10s
user    0m 0.59s
sys     0m 1.02s
---------------------------------------------
Résultats finaux:
Total jetons produits: 5000
Total jetons consommés: 5000
Bilan: 0 jetons non consommés
SUCCÈS: Aucun jeton perdu!
real    0m 1.64s
user    0m 1.26s
sys     0m 2.70s
```
