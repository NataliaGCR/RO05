// arm-linux-g++ -Wall -Wextra -Wno-psabi  td4b.cpp Thread.cpp Semaphore.cpp Mutex.cpp Monitor.cpp timespec.cpp TimeoutException.cpp -o tp4b
#include "Semaphore.h"
#include "Monitor.h"
#include <iostream>
#include <vector>
#include <thread>
#include <atomic>
#include <chrono>

class TokenProducer {
public:
    TokenProducer(Semaphore& sem, int totalTokens) 
        : semaphore(sem), totalTokens(totalTokens), producedCount(0) {}

    void operator()() {
        for (int i = 0; i < totalTokens; ++i) {
            semaphore.give();
            producedCount++;
        }
    }

    int getProducedCount() const { return producedCount; }

private:
    Semaphore& semaphore;
    int totalTokens;
    int producedCount;
};

class TokenConsumer {
public:
    TokenConsumer(Semaphore& sem) 
        : semaphore(sem), consumedCount(0) {}

    void operator()() {
        while (true) {
            if (!semaphore.take(500)) { 
                break; 
            }
            consumedCount++;
        }
    }

    int getConsumedCount() const { return consumedCount; }

private:
    Semaphore& semaphore;
    int consumedCount;
};

int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] 
                  << " <num_consumers> <num_producers> <tokens_per_producer>\n";
        return 1;
    }

    const int numConsumers = std::stoi(argv[1]);
    const int numProducers = std::stoi(argv[2]);
    const int tokensPerProducer = std::stoi(argv[3]);

    Semaphore semaphore(0); 
    std::vector<std::thread> consumers;
    std::vector<std::thread> producers;
    std::vector<TokenConsumer> consumerObjs(numConsumers, TokenConsumer(semaphore));
    std::vector<TokenProducer> producerObjs(numProducers, TokenProducer(semaphore, tokensPerProducer));

    for (int i = 0; i < numConsumers; ++i) {
        consumers.emplace_back(std::ref(consumerObjs[i]));
    }

    for (int i = 0; i < numProducers; ++i) {
        producers.emplace_back(std::ref(producerObjs[i]));
    }

    for (auto& producer : producers) {
        producer.join();
    }

    for (auto& consumer : consumers) {
        consumer.join();
    }

    int totalProduced = 0;
    for (const auto& producer : producerObjs) {
        totalProduced += producer.getProducedCount();
    }

    int totalConsumed = 0;
    for (const auto& consumer : consumerObjs) {
        totalConsumed += consumer.getConsumedCount();
    }

    std::cout << "\nRésultats finaux:\n";
    std::cout << "Total jetons produits: " << totalProduced << "\n";
    std::cout << "Total jetons consommés: " << totalConsumed << "\n";
    std::cout << "Bilan: " << (totalProduced - totalConsumed) 
              << " jetons non consommés\n";

    if (totalProduced == totalConsumed) {
        std::cout << "SUCCÈS: Aucun jeton perdu!\n";
    } else {
        std::cout << "ERREUR: Jetons perdus ou non consommés!\n";
    }

    return 0;
}