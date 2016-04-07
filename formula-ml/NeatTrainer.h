#ifndef NEAT_TRAINER
#define NEAT_TRAINER

#include <neural/neat/Pool.h>
#include <neural/neat/Network.h>
#include <thread>
#include <mutex>
#include <sim/Simulator.h>
#include <neural/FileWriter.h>


class NeatEvaluator
{
public:
    virtual float evaluate_network(neural::Network* network) = 0;
};

class NeatTrainer
{
public:
    NeatTrainer();
	NeatTrainer(string path);
    ~NeatTrainer();
    void run();

	neat::Genome* get_best();
	void set_best(neat::Genome& genome);

    // Mandatory
    std::function<NeatEvaluator*()> evaluator_factory;

    // Optional
    std::function<void(neural::Network* new_best, float fitness)> on_new_best;
    std::function<void(int generation)> on_generation_done;

private:
    void evaluate(neat::Genome& genome, NeatEvaluator* eval);
    void evaluate_thread(NeatEvaluator* eval);
	std::string getTimestamp();

    void showBest();
    bool improved = false;
	int generation;

    neat::Network *bestNetwork;
	neat::Genome *bestGenome;
	std::mutex best_genome_mutex;

    std::mutex mtx;
    std::vector<neat::Genome*> active_genomes;
	std::string savePath;

	neat::Pool * pool;
	neural::FileWriter * fw;
};

#endif