#include "NeatTrainer.h"
#include <core/util/Random.h>
#include <iostream>
#include <core/Window.h>
#include <sim/Simulator.h>
#include <neural/EvolvingNetwork.h>
#include <sim/SimulationState.h>
#include <experiments/StandardRenderer.h>
#include <neural/neat/Constants.h>
#include <thread>

using namespace neat;
using namespace std;

NeatTrainer::NeatTrainer()
{
    Config::set_config(14, 2);
    pool = new Pool();
}


NeatTrainer::~NeatTrainer()
{
    if (pool) delete pool;
}

float fitness(SimulationResult result, float termination_distance, float maximum_time) {
    float fitness = result.distance_driven;

    // If the car has come to the end of the track, let the time
    if (glm::abs(result.distance_driven - termination_distance) < 1) {
        // (sqrt(t_max) - sqrt(t))^2 gives increases the fitness for lower times with
        // higher resolution at lower values. *5 stretches the result to equal the
        // size of distance somewhat.
        float root_difference = glm::sqrt(maximum_time) - glm::sqrt(result.time_alive);
        float time_fitness = root_difference * root_difference * 5.f;

        // If the car would drive slower than the maximum_time constant, make the time a penalty instead
        fitness += result.time_alive < maximum_time ? time_fitness : -time_fitness;
    }

    return fitness;
}

void NeatTrainer::evaluate(Genome& genome, Simulator * sim) {
    const float termination_distance = 5700.f;
    const float maximum_time = 2000.f;
    static int nbr_of_checkpoints = 10;
    static int nbr_of_inputs = 3 + 1 + Simulator::write_track_curve_size(nbr_of_checkpoints);

    float * inputs = new float[nbr_of_inputs];
    float * outputs = new float[Config::Outputs];
    Network *n = new Network(genome.genes);

    // Define struct for ai indata
    neural::NetworkIO network_indata = neural::NetworkIO();
    network_indata.value_count = nbr_of_inputs;
    network_indata.values = inputs;
    neural::NetworkIO output = neural::NetworkIO();
    output.value_count = Config::Outputs;
    output.values = outputs;

    sim->carUpdater = [&]() {
        int i = 0;
        inputs[i++] = sim->distance_to_middle();
        inputs[i++] = sim->angle_to_line();
        inputs[i++] = sim->car->getSpeed();
        sim->write_track_curve(inputs, i, nbr_of_checkpoints);
        inputs[i++] = 1.0f;

        n->fire(network_indata, output);

        CarControl control;
        control.acceleration = 0;
        control.brake = outputs[1];
        control.steer = outputs[0];

        return control;
    };
    SimulationResult result = sim->run(0.01f);

    genome.fitness = fitness(result, termination_distance, maximum_time);
    if (genome.fitness > pool->maxFitness) {
        pool->maxFitness = genome.fitness;
        delete best;
        best = n;
        cout << "New maximum fitness: " << genome.fitness << endl
             << "Distance: " << result.distance_driven << endl
             << "Time: " << result.time_alive << endl << endl;
        improved = true;
    }
    else {
        delete n;
    }
    delete[] inputs;
    delete[] outputs;
//    delete cm;
//    delete tm;
}


void NeatTrainer::showBest() {
    Simulator * sim = new Simulator();
    // Create simulated objects
    // NOTE: Starting grid is at first "checkpoint". In order
    //       to change this, offset the checkpoint order.
    sim->track = new TrackModel(glm::vec3(35.169220, -702.223755, 5.000004));
    sim->car = new CarModel(sim->track->get_start_grid_pos(),
                            glm::vec3(-0.616278410f, -0.787541449f, 0),
                            12.f);


    // Place car at the tracks starting grid.
    sim->car->position = sim->track->get_start_grid_pos();
    sim->car->setSpeed(12.f);
    sim->car->max_speed = 12.f;
    sim->progress_timeout = 0.1f;

    static int nbr_of_checkpoints = 10;
    static int nbr_of_inputs = 3 + 1 + Simulator::write_track_curve_size(nbr_of_checkpoints);

    float * in = new float[nbr_of_inputs];
    float * out = new float[Config::Outputs];
    // Define struct for ai indata
    neural::NetworkIO network_indata = neural::NetworkIO();
    network_indata.value_count = nbr_of_inputs;
    network_indata.values = in;
    neural::NetworkIO output = neural::NetworkIO();
    output.value_count = Config::Outputs;
    output.values = out;

    sim->carUpdater = [&]() {

        int i = 0;
        network_indata.values[i++] = sim->distance_to_middle();
        network_indata.values[i++] = sim->angle_to_line();
        network_indata.values[i++] = sim->car->getSpeed();
        sim->write_track_curve(network_indata.values, i, nbr_of_checkpoints);
        network_indata.values[i++] = 1.0f;

        best->fire(network_indata, output);

        CarControl control;
        control.acceleration = 0;
        control.brake = out[1];
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

void NeatTrainer::evaluate_thread(Simulator * sim) {
    bool run_thread = true;
    while(run_thread) {
        mtx.lock();
        if (active_genomes.size() > 0) {
            neat::Genome * genome = active_genomes.at(0);
            active_genomes.erase(active_genomes.begin());
            mtx.unlock();

            sim->reset();
            sim->car->setSpeed(12.f);
            evaluate(*genome, sim);
        } else {
            run_thread = false;
            mtx.unlock();
        }
    }
}

Simulator* create_simulator() {
    const float termination_distance = 5700.f;

    Simulator * sim = new Simulator();
    // Create simulated objects
    // NOTE: Starting grid is at first "checkpoint". In order
    //       to change this, offset the checkpoint order.
    TrackModel * tm = new TrackModel(glm::vec3(35.169220, -702.223755, 5.000004));
    sim->track = tm;
    CarModel * cm = new CarModel(sim->track->get_start_grid_pos(),
                                 glm::vec3(-0.616278410f, -0.787541449f, 0),
                                 12.f);
    sim->car = cm;

    // Place car at the tracks starting grid.
    sim->car->setSpeed(12.f);
    sim->progress_timeout = 0.1f;
    sim->termination_distance = termination_distance;

    return sim;
}

void NeatTrainer::run() {
    std::cout << "running" << std::endl;
    int thread_count = std::thread::hardware_concurrency();
	std::thread *thread_pool = new std::thread[thread_count];
    Simulator *sim_pool[thread_count];

    std::cout << "creating simulators" << std::endl;
    for (int i = 0; i < thread_count; ++i) {
        sim_pool[i] = create_simulator();
    }
    std::cout << "created simulators" << std::endl;

	int i = 0;
    std::cout << "starting training" << std::endl;
	while (true) {
        std::cout << "training" << std::endl;
		// Build a vector with pointers to all genomes within this generation.
		for (auto && species : pool->species) {
			for (auto && genome : species.genomes) {
				active_genomes.push_back(&genome);
			}
		}

		// Start as many threads as we have processors in order to evaluate more
		// efficiently.
		for (int i = 0; i < thread_count; i++) {
			thread_pool[i] = std::thread(&NeatTrainer::evaluate_thread, this, sim_pool[i]);
			//thread_pool.push_back(std::thread(&NeatTrainer::evaluate_thread, this));
		}

		// Wait for all threads evaluating, since next generation will
		// depend on this generation.
		for (int i = 0; i < thread_count; i++) {
			thread_pool[i].join();
			//thread_pool.erase(thread_pool.begin() + i);
		}

		i++;
		pool->new_generation();
		cout << "New Generation: " << i << endl;
		if (improved) {
			showBest();
			improved = false;
		}
	}
	delete[] thread_pool;
    for (int i = 0; i < thread_count; ++i) {
        delete sim_pool[i];
    }
}
