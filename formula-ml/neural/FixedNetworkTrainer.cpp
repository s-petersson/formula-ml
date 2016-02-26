#include <iostream>
#include <glm/gtx/vector_angle.hpp>
#include <sim/Simulator.h>
#include <sim/model/CarModel.h>
#include "FixedNetworkTrainer.h"

using namespace neural;

FixedNetworkTrainer::FixedNetworkTrainer() {
    track = new TrackModel(glm::vec3(35.169220, -702.223755, 5.000004));
    network = new FixedNetwork(2, 1, 1, 2);
    bestGenome = FixedNetworkGenome();
    bestResult = SimulationResult();
}
FixedNetworkTrainer::~FixedNetworkTrainer() {
    delete track;
    delete network;
}

/*
AI training, compare to mid line, fixed topology, fixed speed
*/
void FixedNetworkTrainer::run() {
    TrackModel* track = new TrackModel(glm::vec3(35.169220, -702.223755, 5.000004));

    FixedNetworkGenome genome = FixedNetworkGenome();

    int gene_steps = 3;
    // total number of iterations: gene_steps ^ size(genome). 
    // gene_steps. 2^6=64, 3^6=729, 4^6=4096, 5^6=15 625, 6^6=46 656, 7^6=117 649, 8^6=262 144, 9^6=531 441, 10^6=1 000 000

    enumerate(genome, 6, gene_steps);
    /*
    bestGenome.genes[0] = 1;
    bestGenome.genes[1] = 0;
    bestGenome.genes[2] = 1;
    bestGenome.genes[3] = 0;
    bestGenome.genes[4] = 0;
    bestGenome.genes[5] = 0;
    /**/

    // TODO: Present the best result
    cout << "\n\n"
        << "----  Best result  ---- \n"
        << "Distance: " << bestResult.distance_driven << "\n"
        << "Time alive: " << bestResult.time_alive << "\n"
        << "Genome: "
        << bestGenome.genes[0] << " "
        << bestGenome.genes[1] << " "
        << bestGenome.genes[2] << " "
        << bestGenome.genes[3] << " "
        << bestGenome.genes[4] << " "
        << bestGenome.genes[5] << "\n\n";
}


/*
Required: gene_steps > 1
*/
void FixedNetworkTrainer::enumerate(FixedNetworkGenome& genome, int number_of_genes, int gene_steps) {
    if (number_of_genes <= 0) {
        testGenome(genome);
        return;
    }
    for (int i = 0; i < gene_steps; i++) {
        // Produce genes in the range [0,1]
        genome.genes[number_of_genes - 1] = i / (float)(gene_steps - 1);
        enumerate(genome, number_of_genes - 1, gene_steps);
    }
}

int iterationCount = 0;
SimulationResult FixedNetworkTrainer::testGenome(FixedNetworkGenome& genome) {
    buildNetwork(genome);

    iterationCount++;
    if (iterationCount % 100 == 0) {
        cout << "Run simulation test " << iterationCount << "\n";
    }

    SimulationResult result = runSimulation(network, track);

    if (result.distance_driven > bestResult.distance_driven) {
        bestResult = result;
        bestGenome = genome;
        cout << "New best result: " << result.distance_driven << "\n" 
            << "Iteration:" << iterationCount << "\n"
            << "Genome: "
            << genome.genes[0] << " "
            << genome.genes[1] << " "
            << genome.genes[2] << " "
            << genome.genes[3] << " "
            << genome.genes[4] << " "
            << genome.genes[5] << "\n\n";
    }

    return result;
}

SimulationResult FixedNetworkTrainer::runSimulation(neural::Network* network, TrackModel* track) {
	Simulator* sim = new Simulator();
    CarModel carModel = CarModel();
	sim->track = track;
	sim->car = &carModel;

	// Place car with speed at the tracks starting grid.
	sim->car->position = sim->track->get_start_grid_pos();
	sim->car->setSpeed(15.f);
	sim->progress_timeout = 0.1f;

	// Define struct for ai indata
	neural::NetworkIO network_indata = neural::NetworkIO();
	network_indata.value_count = 2;
	network_indata.values = new float[network_indata.value_count];

    sim->carUpdater = [&]() {
        setLineData(&network_indata, 0, sim->car, sim->track);
        NetworkIO output = network->fire(network_indata);
        return makeCarControl(output);
    };


	SimulationResult result = sim->run(0.1f);
    delete sim;
	delete network_indata.values;
	return result;
}

void FixedNetworkTrainer::setLineData(NetworkIO* network_indata, int offset, CarModel* car, TrackModel* track) {
    glm::vec3 last_checkpoint = track->get_checkpoints()[glm::max(car->checkpoint - 1, 0)].middle;
    glm::vec3 next_checkpoint = track->get_checkpoints()[glm::max(car->checkpoint, 0)].middle;
    glm::vec3 car_position = car->position;

    // move frame of reference to last_checkpoint
    next_checkpoint -= last_checkpoint;
    car_position -= last_checkpoint;
    glm::vec3 next_checkpoint_normalized = glm::normalize(next_checkpoint);

    // Find distance to line
    float projected_distance_on_line = glm::dot(car_position, next_checkpoint_normalized);
    float distance_to_line = glm::length(car_position - (projected_distance_on_line * next_checkpoint_normalized));

    // Find angle to line
    //float angle_to_line = glm::angle(next_checkpoint, sim->car->direction);
    float angle_to_line = glm::acos(glm::dot(next_checkpoint, car->direction) / glm::length(next_checkpoint) * glm::length(car->direction));
    if (glm::cross(glm::vec3(0, 1, 0), car->direction).z > 0)
        angle_to_line = -angle_to_line;

    network_indata->values[0] = distance_to_line;
    network_indata->values[1] = angle_to_line;
}

float span(float stochastic, float from, float to) {
    return (to - from)*stochastic + from;
}

void FixedNetworkTrainer::buildNetwork(FixedNetworkGenome& genome) {
    network->set_input_weight(0, 0, span(genome.genes[0], -1, 1));
    network->set_input_weight(1, 0, span(genome.genes[1], -1, 1));
    network->set_input_weight(0, 1, span(genome.genes[2], -1, 1));
    network->set_input_weight(1, 1, span(genome.genes[3], -1, 1));
    network->set_output_weight(0, 0, span(genome.genes[4], -1, 1));
    network->set_output_weight(1, 0, span(genome.genes[5], -1, 1));
}

CarControl FixedNetworkTrainer::makeCarControl(NetworkIO output) {
    CarControl carControl = CarControl();

    // TODO Insert threashhold for steering
    carControl.steer = span(output.values[0], -0.1, 0.1);
    return carControl;
}
