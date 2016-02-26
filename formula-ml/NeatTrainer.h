#ifndef NEAT_TRAINER
#define NEAT_TRAINER

#include <neural/neat/Pool.h>
#include "neural/neat/Network.h"

class NeatTrainer
{
public:
    NeatTrainer();
    ~NeatTrainer();

    neural::Network* train();

private:
    void evaluateFitness(neat::Genome& genome);
    neat::Pool * pool;
    neat::Network best;
};

#endif