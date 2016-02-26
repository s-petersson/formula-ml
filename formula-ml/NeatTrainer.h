#ifndef NEAT_TRAINER
#define NEAT_TRAINER

#include <neural/neat/Pool.h>

class NeatTrainer
{
public:
    NeatTrainer();
    ~NeatTrainer();

    void train();

private:
    void evaluateFitness(neat::Genome& genome);
    neat::Pool * pool;
};

#endif