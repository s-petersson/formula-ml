#include "XORExperiment.h"
#include <neural/neat/Constants.h>
#include <iostream>


using namespace neat;
using namespace std;
XORExperiment::XORExperiment()
{

    Config::set_config(2, 1);

    trainer = new NeatTrainer();
   
    trainer->evaluator_factory = []() {
        return new XOREvaluator;
    };

    trainer->on_generation_done = [](int generation){
        cout << "New Generation: " << generation << endl;
    };

    trainer->on_new_best = [](neat::Genome* new_best, float fitness){
        cout << "New best fitness: " << fitness << endl;
    };
}

XORExperiment::~XORExperiment()
{
    if (trainer) delete trainer;
}

void XORExperiment::run() {
    
    cout << "NEAT XOR EXPERIMENT" << endl;
    trainer->run();
}

float XOREvaluator::evaluate_network(neat::Network* network) {
    // Network IO init
    neural::NetworkIO *in, *out;
    in = new neural::NetworkIO;
    out = new neural::NetworkIO;
    in->value_count = 2;
    in->values = new float[in->value_count];
    out->value_count = 1;
    out->values = new float[out->value_count];
    
    float fitness = 0.0f;

    in->values[0] = 0.0f;
    in->values[1] = 0.0f;
    network->fire(*in, *out);
    fitness += (1.0f - out->values[0]);

    in->values[0] = 0.0f;
    in->values[1] = 1.0f;
    network->fire(*in, *out);
    fitness += out->values[0];

    in->values[0] = 1.0f;
    in->values[1] = 0.0f;
    network->fire(*in, *out);
    fitness += out->values[0];

    in->values[0] = 1.0f;
    in->values[1] = 1.0f;
    network->fire(*in, *out);
    fitness += (1.0f - out->values[0]);

    fitness = fitness * fitness;

    // Cleanup and return
    delete[] in->values;
    delete[] out->values;
    delete in;
    delete out;
    return fitness;
}