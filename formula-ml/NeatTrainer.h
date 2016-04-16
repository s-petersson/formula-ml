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
    virtual float evaluate_network(neat::Network* network) = 0; // also take memory ownage of network
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

    // Mandatory parameters
    std::function<NeatEvaluator*()> evaluator_factory;

    // Optional parameters
    std::function<void(neat::Genome * new_best, float fitness)> on_new_best;
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