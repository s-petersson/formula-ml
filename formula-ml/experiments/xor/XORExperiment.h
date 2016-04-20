#ifndef EXPERIMENTS_XOR
#define EXPERIMENTS_XOR

#include <neural/neat/Trainer.h>
#include <experiments/Experiment.h>


class XORExperiment : public Experiment
{
public:
    XORExperiment();
    ~XORExperiment();

    virtual void run();

private:
    neat::Trainer* trainer;
};

class XOREvaluator : public neat::Evaluator
{
public:
    virtual float evaluate_network(neat::Network* network);
};

#endif