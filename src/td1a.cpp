// arm-linux-g++ -Wall -Wextra -Wno-psabi td1a.cpp timespec.cpp -o td1a
#include "timespec.h"

#include <iostream>
#include <unistd.h>
#include <time.h>

timespec timespec_now();
timespec timespec_negate(const timespec& time_ts);
timespec timespec_add(const timespec& time1_ts, const timespec& time2_ts);
timespec timespec_subtract(const timespec& time1_ts, const timespec& time2_ts);
timespec timespec_from_ms(double time_ms);
double timespec_to_ms(const timespec& time_ts);
void timespec_wait(const timespec& delay_ts);


int main() {
    timespec tsp1 = {2, 500000000};  // 2.5 segundos
    double ms = timespec_to_ms(tsp1);
    std::cout << "Timespec en millisecondes: " << ms << " ms" << std::endl;

    timespec tsp3 = timespec_from_ms(ms);
    std::cout << "Millisecondes à Timespec: " << tsp3.tv_sec << " s, " << tsp3.tv_nsec << " nsec" << std::endl;

    timespec current_time = timespec_now();
    std::cout << "Temps actuelle: " << current_time.tv_sec << " s, " << current_time.tv_nsec << " nsec" << std::endl;    
    timespec tsp_neg = timespec_negate(tsp1);
    std::cout << "Négatif de " << tsp1.tv_sec << " s, " << tsp1.tv_nsec << " nsec est: "
              << tsp_neg.tv_sec << " s, " << tsp_neg.tv_nsec << " nsec" << std::endl; 
    
    timespec tsp2 = {1, 700000000}; //1.7 segundos
    timespec tsp_add = timespec_add(tsp1, tsp2);
    std::cout << "Somme de " << tsp1.tv_sec << " s, " << tsp1.tv_nsec << " nsec y "
              << tsp2.tv_sec << " s, " << tsp2.tv_nsec << " nsec: "
              << tsp_add.tv_sec << " s, " << tsp_add.tv_nsec << " nsec" << std::endl;
    
    timespec tsp_sub = timespec_subtrac(tsp1, tsp2);
    std::cout << "Reste de " << tsp1.tv_sec << " s, " << tsp1.tv_nsec << " nsec y "
              << tsp2.tv_sec << " s, " << tsp2.tv_nsec << " nsec: "
              << tsp_sub.tv_sec << " s, " << tsp_sub.tv_nsec << " nsec" << std::endl;

    timespec delay = {1, 80000000};  // 1.8 segundo
    std::cout << "En attente " << delay.tv_sec << " secondes(s)..." << std::endl;
    timespec_wait(delay);


    timespec t1 = {5, 200000000};  // 5.2 segundos
    timespec t2 = {3, 500000000};  // 3.5 segundos
    

    timespec result_add = t1 + t2;
    std::cout << "Somme (t1 + t2): "  << result_add.tv_sec << " s, " << result_add.tv_nsec << " nsec" << std::endl;

    timespec result_sub = t1 - t2;
    std::cout <<"Reste (t1 - t2): "  <<result_sub.tv_sec << " s, " << result_sub.tv_nsec << " nsec" << std::endl;

    timespec result_neg = -t1;
    std::cout << "Negaf de t1: - " << result_neg.tv_sec << " s, " << result_neg.tv_nsec << " nsec" << std::endl;

    t1 += t2;
    std::cout << "t1 += t2: "  << t1.tv_sec << " s, " << t1.tv_nsec << " nsec" << std::endl;
    
    t1 -= t2;
    std::cout << "t1 -= t2: "  << t1.tv_sec << " s, " << t1.tv_nsec << " nsec" << std::endl;

    if (t1 == t2) {
        std::cout << "t1 est égal à t2" << std::endl;
    } else {
        std::cout << "t1 n'est pas égal à t2" << std::endl;
    }

    if (t1 != t2) {
        std::cout << "t1 est différent de t2" << std::endl;
    }

    if (t1 < t2) {
        std::cout << "t1 est inférieur à t2" << std::endl;
    }

    if (t1 > t2) {
        std::cout << "t1 est supérieur à t2" << std::endl;
    } 
    return 0;
}
