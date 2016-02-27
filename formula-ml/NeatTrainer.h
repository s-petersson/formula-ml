#ifndef NEAT_TRAINER
#define NEAT_TRAINER

#include <neural/neat/Pool.h>
#include <neural/neat/Network.h>

class NeatTrainer
{
public:
    NeatTrainer();
    ~NeatTrainer();

    void run();

private:
    void evaluate(neat::Genome& genome);
    void showBest();
    bool improved = false;
    neat::Pool * pool;
    neat::Network *best;
};

#endif