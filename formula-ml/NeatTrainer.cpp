#include "NeatTrainer.h"
#include <core/util/Random.h>
#include <iostream>
#include <core/Window.h>
#include <sim/Simulator.h>
#include <neural/EvolvingNetwork.h>
#include <sim/SimulationState.h>
#include <experiments/StandardRenderer.h>
#include <neural/neat/Constants.h>

using namespace neat;
using namespace std;

NeatTrainer::NeatTrainer()
{
    Config::set_config(13, 1);
    pool = new Pool();
}


NeatTrainer::~NeatTrainer()
{
    if (pool) delete pool;
}



void NeatTrainer::evaluate(Genome& genome) {
    static int nbr_of_checkpoints = 5;
    static int nbr_of_inputs = 3 + Simulator::write_track_curve_size(nbr_of_checkpoints);

    float * inputs = new float[nbr_of_inputs];
    float * outputs = new float[1];
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
    sim->car->setSpeed(5.f);
    sim->progress_timeout = 0.1f;

    // Define struct for ai indata
    neural::NetworkIO network_indata = neural::NetworkIO();
    network_indata.value_count = nbr_of_inputs;
    network_indata.values = inputs;
    neural::NetworkIO output = neural::NetworkIO();
    output.value_count = 1;
    output.values = outputs;

    sim->carUpdater = [&]() {

        int i = 0;
        inputs[i++] = sim->distance_to_middle();
        inputs[i++] = sim->angle_to_line();
        sim->write_track_curve(inputs, i, nbr_of_checkpoints);
        inputs[i++] = 1.0f;


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
    sim->car->setSpeed(5.f);
    sim->progress_timeout = 0.1f;

    static int nbr_of_checkpoints = 5;
    static int nbr_of_inputs = 3 + Simulator::write_track_curve_size(nbr_of_checkpoints);

    float * in = new float[nbr_of_inputs];
    float * out = new float[1];
    // Define struct for ai indata
    neural::NetworkIO network_indata = neural::NetworkIO();
    network_indata.value_count = nbr_of_inputs;
    network_indata.values = in;
    neural::NetworkIO output = neural::NetworkIO();
    output.value_count = 1;
    output.values = out;

    sim->carUpdater = [&]() {

        int i = 0;
        network_indata.values[i++] = sim->distance_to_middle();
        network_indata.values[i++] = sim->angle_to_line();
        sim->write_track_curve(network_indata.values, i, nbr_of_checkpoints);
        network_indata.values[i++] = 1.0f;

        best->fire(network_indata, output);

        CarControl control;
        control.acceleration = 0;
        control.brake = 0;
        control.steer = out[0];

        return control;
    };
    Window * window = new Window();
    vector<Renderer*> renderers;
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
