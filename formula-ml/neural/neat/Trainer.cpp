#include "Trainer.h"
#include <neural/neat/Constants.h>
#include <neural/Helpers.h>
#include <core/util/Util.h>
#include <thread>
#include <sstream>
#include <iostream>

using namespace neat;
using namespace std;


std::string getTimestamp() {
	std::stringstream stamp;
	stamp << current_time();
	return stamp.str();
}

Trainer::Trainer()
{	
    cout << "Current Timestamp: " << getTimestamp() << std::endl;
	fw = new neural::FileWriter("saves/" + getTimestamp() + "/");
	pool = new Pool();
	pool->fill();
    bestGenome = new Genome();
}

Trainer::Trainer(string path) {
	fw = new neural::FileWriter("saves/" + getTimestamp()+ "/");
	pool = (*fw).poolFromFile(path);
    bestGenome = new Genome();
}

Trainer::~Trainer()
{
    if (pool) delete pool;
	if (fw) delete fw;
}



void Trainer::evaluate(Genome& genome, Evaluator* evaluator) {
    Network *n = new Network(genome.genes);
    genome.fitness = evaluator->evaluate_network(n);

    if (genome.fitness > pool->maxFitness) {
		std::ostringstream oss;
		std::string path = oss.str();
		set_best(genome);
        on_new_best(&genome, bestGenome->fitness);
    }

    delete n;
}

void Trainer::evaluate_thread(Evaluator* evaluator) {
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

void Trainer::run() {
    int thread_count = std::thread::hardware_concurrency();
	std::thread *thread_pool = new std::thread[thread_count];
	Evaluator **eval_pool = new Evaluator*[thread_count];

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
			thread_pool[i] = std::thread(&Trainer::evaluate_thread, this, eval_pool[i]);
		}

		// Wait for all threads evaluating, since next generation will
		// depend on this generation.
		for (int i = 0; i < thread_count; i++) {
			thread_pool[i].join();
		}

		i++;
		generation = i;

        if (on_generation_done) {
            on_generation_done(generation);
        }

		pool->new_generation();

		if (improved) {
			(*fw).poolToFile(*pool, generation);
			improved = false;
		}
	}
	delete[] thread_pool;
    for (int i = 0; i < thread_count; ++i) {
        delete eval_pool[i];
    }
}

neat::Genome* Trainer::get_best() {
    best_genome_mutex.lock();
    Genome * temp;
    if (bestGenome) {
        temp = new Genome(*bestGenome);
    } else {
        temp = nullptr;
    }
    best_genome_mutex.unlock();
    return temp;
}

void Trainer::set_best(neat::Genome& genome) {
	best_genome_mutex.lock();
	if (bestGenome == nullptr || genome.fitness > bestGenome->fitness) {
		if(bestGenome != nullptr) delete bestGenome;
		bestGenome = new Genome(genome);
		pool->maxFitness = genome.fitness;
		improved = true;
	}
	best_genome_mutex.unlock();
}
