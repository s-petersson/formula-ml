#include <glm/gtx/vector_angle.hpp>
#include <sim/Simulator.h>
#include "FixedNetworkTrainer.h"

using namespace neural;

FixedNetworkTrainer::~FixedNetworkTrainer() {

}
FixedNetworkTrainer::FixedNetworkTrainer() {

}

struct Genome {
	float genes[6];
};

void testGenome(Genome& genome) {
	// TODO:
	// build network
	// runSimulation()
	// update best
}

/*
Required: gene_steps > 1
*/
void enumerate(Genome& genome, int number_of_genes, int gene_steps) {
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

/* 
	AI training, compare to mid line, fixed topology, fixed speed
*/
void FixedNetworkTrainer::run() {
	TrackModel* track = new TrackModel(glm::vec3(35.169220, -702.223755, 5.000004));

	Genome genome = Genome();

	int gene_steps = 4; 
	// total number of iterations: gene_steps ^ size(genome). 
	// gene_steps. 2^6=64, 3^6=729, 4^6=4096, 5^6=15 625, 6^6=46 656, 7^6=117 649, 8^6=262 144, 9^6=531 441, 10^6=1 000 000

	enumerate(genome, 6, gene_steps);

	// TODO: Present the best result
}


SimulationResult runSimulation(neural::Network& network, TrackModel* track) {
	Simulator * sim = new Simulator();
	sim->track = track;
	sim->car = new CarModel();

	// Place car with speed at the tracks starting grid.
	sim->car->position = sim->track->get_start_grid_pos();
	sim->car->setSpeed(20);
	sim->progress_timeout = 1;

	// Define struct for ai indata
	neural::NetworkIO network_indata = neural::NetworkIO();
	network_indata.value_count = 2;
	network_indata.values = new float[network_indata.value_count];

	sim->carUpdater = [&]() {
		glm::vec3 last_checkpoint = sim->track->get_checkpoints()[glm::max(sim->car->checkpoint - 1, 0)].middle;
		glm::vec3 next_checkpoint = sim->track->get_checkpoints()[glm::max(sim->car->checkpoint, 0)].middle;
		glm::vec3 car_position = sim->car->position;

		// move frame of reference to last_checkpoint
		next_checkpoint -= last_checkpoint;
		car_position -= last_checkpoint;
		glm::vec3 next_checkpoint_normalized = glm::normalize(next_checkpoint);

		// Find distance to line
		float projected_distance_on_line = glm::dot(car_position, next_checkpoint_normalized);
		float distance_to_line = glm::length(car_position - projected_distance_on_line * next_checkpoint_normalized);

		// Find angle to line
		float angle_to_line = glm::angle(next_checkpoint, sim->car->direction);

		network_indata.values[0] = distance_to_line;
		network_indata.values[1] = angle_to_line;

		NetworkIO output = network.fire(network_indata);

		CarControl carControl = CarControl();
		carControl.steer = output.values[0];

		return carControl;
	};

	SimulationResult result = sim->run(0.1f);
	delete network_indata.values;
	return result;
}
