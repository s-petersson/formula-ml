#include "NeatTrainer.h"
#include <core/util/Random.h>
#include <iostream>

using namespace neat;
using namespace std;

NeatTrainer::NeatTrainer()
{
    pool = new Pool();
}


NeatTrainer::~NeatTrainer()
{
    if (pool) delete pool;
}


void NeatTrainer::train(){
    while (true) {
    for (auto && species : pool->species) {
        for (auto && genome : species.genomes) {
            evaluateFitness(genome);
        }
    }
    pool->new_generation();
    cout << "New generation. Number of species: " << pool->species.size() << endl;
}
}

void NeatTrainer::evaluateFitness(Genome& genome) {

    float fitness = 0.0f;
    float * inputs = new float[2];
    float * outputs = new float[1];
    inputs[1] = 1.0f;
    Network n(genome.genes);
    
    for (int i = 0; i < 100; i++) {
        float dist = rngf(0.0f, 2.0f) - 1.0f;
        inputs[0] = dist;
        neural::NetworkIO in, out;
        in = { inputs,2 };
        out = { outputs,1 };
        n.fire(in,out);
        
        if (dist > 0.0f) {
            if (outputs[0] > 0.5f) fitness -= 1.0f;
            if (outputs[0] < -0.5f) fitness += 1.0f;
        }
        if (dist < 0.0f) {
            if (outputs[0] > 0.5f) fitness += 1.0f;
            if (outputs[0] < -0.5f) fitness -= 1.0f;
        }
    }
    genome.fitness = fitness;
    if (fitness > pool->maxFitness) {
        pool->maxFitness = fitness;
        cout << "New maximum fitness: " << fitness << endl;
    }
}