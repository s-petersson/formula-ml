#ifndef NEAT_TRAINER
#define NEAT_TRAINER

#include <neural/neat/Pool.h>
#include <neural/neat/Network.h>
#include <thread>
#include <mutex>

class NeatTrainer
{
public:
    NeatTrainer();
    ~NeatTrainer();

    void run();

private:
    void evaluate(neat::Genome& genome);
    void evaluate_thread();
    void showBest();
    bool improved = false;
    neat::Pool * pool;
    neat::Network *best;

    std::mutex mtx;
    std::vector<neat::Genome*> active_genomes;
};

#endif