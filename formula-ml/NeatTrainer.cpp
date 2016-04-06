#include "NeatTrainer.h"
#include <core/util/Random.h>
#include <iostream>
#include <core/Window.h>
#include <sim/Simulator.h>
#include <neural/EvolvingNetwork.h>
#include <sim/SimulationState.h>
#include <experiments/StandardRenderer.h>
#include <neural/neat/Constants.h>
#include <neural/Helpers.h>
#include <thread>
#include <sstream>

using namespace neat;
using namespace std;

NeatTrainer::NeatTrainer()
{	
	cout << getTimestamp();
	fw = new neural::FileWriter("saves/" + getTimestamp() + "/");
	pool = new Pool();
	pool->fill();
}

NeatTrainer::NeatTrainer(string path) {
	fw = new neural::FileWriter("saves/" + getTimestamp()+ "/");
	pool = (*fw).poolFromFile(path);
}

NeatTrainer::~NeatTrainer()
{
    if (pool) delete pool;
	if (fw) delete fw;
}

std::string NeatTrainer::getTimestamp() {
	std::stringstream stamp;
	stamp << "Current Timestamp: " << current_time() << std::endl;
	return stamp.str();
}

void NeatTrainer::evaluate(Genome& genome, NeatEvaluator* evaluator) {
    Network *n = new Network(genome.genes);
    genome.fitness = evaluator->evaluate_network(n);

    if (genome.fitness > pool->maxFitness) {
		std::ostringstream oss;
		//oss << savePath << "Generation" << generation <<".txt";
		std::string path = oss.str();
		set_best(genome);
    }
    else {
        delete n;
    }
//    delete cm;
//    delete tm;
}

void NeatTrainer::evaluate_thread(NeatEvaluator* evaluator) {
    bool run_thread = true;
    while(run_thread) {
        mtx.lock();
        if (active_genomes.size() > 0) {
            neat::Genome * genome = active_genomes.at(0);
            active_genomes.erase(active_genomes.begin());
            mtx.unlock();

            evaluate(*genome, evaluator);
        } else {
            run_thread = false;
            mtx.unlock();
        }
    }
}

void NeatTrainer::run() {
    int thread_count = std::thread::hardware_concurrency();
	std::thread *thread_pool = new std::thread[thread_count];
	NeatEvaluator **eval_pool = new NeatEvaluator*[thread_count];

    for (int i = 0; i < thread_count; ++i) {
        eval_pool[i] = evaluator_factory();
    }

	int  i = 0;
	while (true) {
		// Build a vector with pointers to all genomes within this generation.
		for (auto && species : pool->species) {
			for (auto && genome : species.genomes) {
				active_genomes.push_back(&genome);
			}
		}

		// Start as many threads as we have processors in order to evaluate more
		// efficiently.
		for (int i = 0; i < thread_count; i++) {
			thread_pool[i] = std::thread(&NeatTrainer::evaluate_thread, this, eval_pool[i]);
			//thread_pool.push_back(std::thread(&NeatTrainer::evaluate_thread, this));
		}

		// Wait for all threads evaluating, since next generation will
		// depend on this generation.
		for (int i = 0; i < thread_count; i++) {
			thread_pool[i].join();
			//thread_pool.erase(thread_pool.begin() + i);
		}

		i++;
		generation = i;

        if (on_generation_done) {
            on_generation_done(generation);
        }

		pool->new_generation();

		if (improved) {
			/*ostringstream path;
			path << "Generation_" << generation;
			*/
			(*fw).poolToFile(*pool, generation);
			//(*fw).genomeToFile(*bestGenome, generation);

            if (on_new_best) {
                neural::Network * network = new Network(bestGenome->genes);
                on_new_best(network, bestGenome->fitness);
            }

			improved = false;
		}
	}
	delete[] thread_pool;
    for (int i = 0; i < thread_count; ++i) {
        delete eval_pool[i];
    }
}



neat::Genome NeatTrainer::get_best() {
    best_genome_mutex.lock();
    Genome temp = Genome(*bestGenome);
    best_genome_mutex.unlock();
    return temp;
}
void NeatTrainer::set_best(neat::Genome& genome) {
	best_genome_mutex.lock();
	if ( bestGenome == nullptr || genome.fitness > bestGenome->fitness) {
		delete bestGenome;
		bestGenome = new Genome(genome);
		pool->maxFitness = genome.fitness;
		improved = true;
	}
	best_genome_mutex.unlock();
}
