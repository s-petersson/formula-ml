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
	stamp << util::current_time();
	return stamp.str();
}

Trainer::Trainer()
{	
    cout << "Current Timestamp: " << getTimestamp() << std::endl;
    savePath = "saves/" + getTimestamp() + "/";
	fw = new neural::FileWriter(savePath);
	pool = new Pool();
	pool->fill();
    best_genome = Genome();
    generation = 0;
}

Trainer::Trainer(string path) {
    savePath = "saves/" + getTimestamp() + "/";
	fw = new neural::FileWriter(savePath);
	pool = (*fw).poolFromFile(path);
    best_genome = Genome();
}

Trainer::~Trainer()
{
    if (pool) delete pool;
	if (fw) delete fw;
}

void Trainer::evaluate(Genome& genome, Evaluator* evaluator) {
    Network *n = new Network(genome.genes);
    EvaluationResult evaluationResult = evaluator->evaluate_network(n);
    genome.fitness = evaluationResult.fitness;

    if (genome.fitness > pool->maxFitness) {
		set_best(genome, evaluationResult);
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

    for (int i = 0; i < thread_count; i++) {
        eval_pool[i] = evaluator_factory();
    }

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

		generation++;;

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

neat::Genome Trainer::get_best() {
    best_genome_mutex.lock();
    Genome temp = best_genome;
    best_genome_mutex.unlock();
    return temp;
}

void Trainer::set_best(neat::Genome& genome, EvaluationResult evaluationResult) {
	best_genome_mutex.lock();
	if (genome.fitness > best_genome.fitness) {
		best_genome = genome;
		pool->maxFitness = genome.fitness;
		improved = true;
        on_new_best(evaluationResult);
	}
	best_genome_mutex.unlock();
}
