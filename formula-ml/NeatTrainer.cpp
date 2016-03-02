#include "NeatTrainer.h"
#include <core/util/Random.h>
#include <iostream>
#include <core/Window.h>
#include <sim/Simulator.h>
#include <neural/EvolvingNetwork.h>
#include <sim/SimulationState.h>
#include <experiments/StandardRenderer.h>

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
    float * inputs = new float[3];
    float * outputs = new float[1];
    inputs[2] = 1.0f;
    Network *n = new Network(genome.genes);

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
    network_indata.value_count = 3;
    network_indata.values = inputs;
    neural::NetworkIO output = neural::NetworkIO();
    output.value_count = 1;
    output.values = outputs;

    sim->carUpdater = [&]() {

        inputs[0] = sim->distance_to_middle();
        inputs[1] = sim->angle_to_line();
        inputs[2] = 1.0f;


        n->fire(network_indata, output);

        CarControl control;
        control.acceleration = 0;
        control.brake = 0;
        control.steer = outputs[0];

        return control;
    };
    SimulationResult result = sim->run(0.01f);

    genome.fitness = result.distance_driven;
    if (genome.fitness > pool->maxFitness) {
        pool->maxFitness = genome.fitness;
        delete best;
        best = n;
        cout << "New maximum fitness: " << genome.fitness << endl;
        improved = true;
    }
    else {
        delete n;
    }
    delete[] inputs;
    delete[] outputs;
    delete cm;
    delete tm;
    delete sim;
}


void NeatTrainer::showBest() {
    Simulator * sim = new Simulator();
    // Create simulated objects
    // NOTE: Starting grid is at first "checkpoint". In order
    //       to change this, offset the checkpoint order.
    sim->track = new TrackModel(glm::vec3(35.169220, -702.223755, 5.000004));
    sim->car = new CarModel();


    // Place car at the tracks starting grid.
    sim->car->position = sim->track->get_start_grid_pos();
    sim->car->setSpeed(15.f);
    sim->progress_timeout = 0.1f;

    float * in = new float[3];
    float * out = new float[1];
    // Define struct for ai indata
    neural::NetworkIO network_indata = neural::NetworkIO();
    network_indata.value_count = 3;
    network_indata.values = in;
    neural::NetworkIO output = neural::NetworkIO();
    output.value_count = 1;
    output.values = out;

    sim->carUpdater = [&]() {

        in[0] = sim->distance_to_middle();
        in[1] = sim->angle_to_line();
        in[2] = 1.0f;

        best->fire(network_indata, output);

        CarControl control;
        control.acceleration = 0;
        control.brake = 0;
        control.steer = out[0];

        return control;
    };
    Window * window = new Window();
    vector<SimulationRenderer*> renderers;
    renderers.push_back(new StandardRenderer(sim));
    SimulationState * s= new SimulationState(sim, renderers);

    window->setState(s);
    window->run();
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
        if (improved) {
            showBest();
            improved = false;
        }
    }
}
