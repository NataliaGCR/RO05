//arm-linux-g++ -Wall -Wextra -Wno-psabi td1b.cpp Chrono.cpp timespec.cpp -o tp1b
#include <iostream>
#include <thread> 
#include "Chrono.h"

int main() {
    Chrono chrono;

    std::cout << "Démarrage du chronomètre..." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(2));

    std::cout << "Temps écoulé après 2 secondes : " << chrono.lap_ms() << " ms" << std::endl;

    chrono.stop();
    std::cout << "Chronomètre arrêté." << std::endl;

    std::this_thread::sleep_for(std::chrono::seconds(2)); 
    std::cout << "Temps écoulé après arrêt : " << chrono.lap_ms() << " ms" << std::endl;
    return 0;
}
