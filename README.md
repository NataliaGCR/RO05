# CSC_5RO05_TA - GALLEGO Natalia

## TD1

### Gestion simplifiée du temps Posix

Compilez les fichiers sources :
   sh
arm-linux-g++ -Wall -Wextra -Wno-psabi td1a.cpp timespec.cpp -o td1a
Sortie :
   sh
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

getSample() doit être const car :

- Il ne fait que lire les données membres (m_iLoop).
- Il ne modifie pas l'état de l'objet.
- Il correspond aux meilleures pratiques C++ pour les méthodes getter.

*Qualification des Variables*

m_iLoop (double) :

- Aucune qualification spéciale nécessaire.
- Accédé/modifié uniquement dans runLoop().
- Pas de exigences de sécurité des threads dans ce cas d'utilisation.

m_doStop (bool) :

Doit être volatile si :

- Utilisé dans un contexte multithread.
- Modifié via des signaux/interruptions.
- Nécessité d'empêcher les optimisations du compilateur.

Pas nécessaire pour un fonctionnement mono-thread.

## Exemple d'Implémentation

cpp
class Looper {
public:
    double getSample() const { return m_iLoop; } // Getter `const` correct
    
private:
    double m_iLoop;         // Membre régulier
    volatile bool m_doStop; // Drapeau prenant en compte les threads
};


Pour le compiler, suivez ces étapes :

1. Naviguez vers le répertoire du projet :
   sh
   cd MyProject/src/

2. Compilez les fichiers sources :
   sh
   arm-linux-g++ -Wall -Wextra td2c.cpp Chrono.cpp TimespecUtils.cpp -o td2c

3. Transférez le fichier compilé vers le dispositif ARM :
   sh
   rsync -avz td2c root@192.168.50.43:

Cela transférera le fichier vers la machine cible avec l'adresse IP 192.168.50.43.

4. Connectez-vous au dispositif ARM via SSH :
   sh
   ssh root@192.168.50.43

5. Exécutez le programme compilé avec différents nombres de boucles :
   sh
   ./td2c 100
   ./td2c 1000
   ./td2c 10000
   ./td2c 100000
   ./td2c 1000000
   ./td2c 10000000
   ./td2c 100000000
   ./td2c 1000000000
   
   
Sortie :
   sh
   nLoops = 100
   Temps d'exécution : 0.007656 millisecondes
   
   nLoops = 1000
   Temps d'exécution : 0.060156 millisecondes
   
   nLoops = 10000
   Temps d'exécution : 0.585469 millisecondes
   
   nLoops = 100000
   Temps d'exécution : 5.915 millisecondes
   
   nLoops = 1000000
   Temps d'exécution : 58.5209 millisecondes
   
   nLoops = 10000000
   Temps d'exécution : 584.381 millisecondes
   
   nLoops = 100000000
   Temps d'exécution : 5843.03 millisecondes
   
   nLoops = 1000000000
   Temps d'exécution : 58428.2 millisecondes
   

![Courbe des valeurs des temps d’exécution en fonction des valeurs de nLoops](TempsExecution_vs_nLoops_TD2C.png)
