#ifndef EXPERIMENTS_XOR
#define EXPERIMENTS_XOR

#include <NeatTrainer.h>
#include <experiments/Experiment.h>


class XORExperiment : public Experiment
{
public:
    XORExperiment();
    ~XORExperiment();

    virtual void run();

private:
    NeatTrainer * trainer;
};

class XOREvaluator : public NeatEvaluator
{
public:
    virtual float evaluate_network(neat::Network* network);
};

#endif