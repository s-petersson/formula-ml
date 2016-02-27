#include "NeatTrainer.h"
#include <core/util/Random.h>
#include <iostream>
#include <core/Window.h>
#include <sim/Simulator.h>
#include <neural/EvolvingNetwork.h>
#include "SimulationState.h"

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



void NeatTrainer::evaluate(Genome& genome) {
    float * inputs = new float[2];
    float * outputs = new float[1];
    inputs[1] = 1.0f;
    Network n(genome.genes);

    Simulator * sim = new Simulator();
    // Create simulated objects
    // NOTE: Starting grid is at first "checkpoint". In order
    //       to change this, offset the checkpoint order.
    TrackModel * tm = new TrackModel(glm::vec3(35.169220, -702.223755, 5.000004));
    sim->track = tm;
    CarModel * cm = new CarModel();
    sim->car = cm;

    // Place car at the tracks starting grid.
    sim->car->position = sim->track->get_start_grid_pos();
    sim->car->setSpeed(15.f);
    sim->progress_timeout = 0.1f;

    // Define struct for ai indata
    neural::NetworkIO network_indata = neural::NetworkIO();
    network_indata.value_count = 2;
    network_indata.values = inputs;
    neural::NetworkIO output = neural::NetworkIO();
    output.value_count = 1;
    output.values = outputs;

    sim->carUpdater = [&]() {

        inputs[0] = sim->distance_to_middle();
        inputs[1] = 1.0f;


        n.fire(network_indata, output);

        CarControl control;
        control.acceleration = 0;
        control.brake = 0;
        control.steer = -outputs[0] * 0.5f;

        return control;
    };
    SimulationResult result = sim->run(0.01f);
    cout << "SIMULATION RESULT " << result.distance_driven << endl;
    
    genome.fitness = result.distance_driven;
    if (genome.fitness > pool->maxFitness) {
        pool->maxFitness = genome.fitness;
        best = n;
        cout << "New maximum fitness: " << genome.fitness << endl;
    }
    delete[] inputs;
    delete[] outputs;
    delete cm;
    delete tm;
    delete sim;
}


void NeatTrainer::showBest() {
    cout << 1 << endl;
    Simulator * sim = new Simulator();
    // Create simulated objects
    // NOTE: Starting grid is at first "checkpoint". In order
    //       to change this, offset the checkpoint order.
    sim->track = new TrackModel(glm::vec3(35.169220, -702.223755, 5.000004));
    sim->car = new CarModel();
    cout << 2 << endl;

    // Place car at the tracks starting grid.
    sim->car->position = sim->track->get_start_grid_pos();
    sim->car->setSpeed(15.f);
    sim->progress_timeout = 0.1f;

    cout << 3 << endl;

    float * in = new float[2];
    float * out = new float[1];
    // Define struct for ai indata
    neural::NetworkIO network_indata = neural::NetworkIO();
    network_indata.value_count = 2;
    network_indata.values = in;
    neural::NetworkIO output = neural::NetworkIO();
    output.value_count = 1;
    output.values = out;
    cout << 4 << endl;
    sim->carUpdater = [&]() {

        in[0] = sim->distance_to_middle();
        in[1] = 1.0f;


        best.fire(network_indata, output);

        CarControl control;
        control.acceleration = 0;
        control.brake = 0;
        control.steer = -out[0] * 0.5f;

        return control;
    };
    cout << 5 << endl;
    Window * window = new Window();
    SimulationState * s = new SimulationState(sim);
    
    window->setState(s);
    window->run();
    cout << 6 << endl;
    delete window;
    delete[] in;
    delete[] out;
    delete sim->car;
    delete sim->track;
    delete sim;
}

void NeatTrainer::run() {
    int i = 0;
    while (true) {
        for (auto && species : pool->species) {
            for (auto && genome : species.genomes) {
                evaluate(genome);
            }
        }
        i++;
        pool->new_generation();
        cout << "New Generation: " << i << endl;
        showBest();
    }
}


neural::Network* NeatTrainer::train(){
    while (true) {
        for (auto && species : pool->species) {
            for (auto && genome : species.genomes) {
                evaluateFitness(genome);
            }
        }
        if (pool->maxFitness >= 100) {
            break;
        }

        pool->new_generation();
        cout << "New generation. Number of species: " << pool->species.size() << endl;
        
    }
    return &best;
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
        best = n;
        cout << "New maximum fitness: " << fitness << endl;
    }
}