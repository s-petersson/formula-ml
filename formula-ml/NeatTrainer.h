#ifndef NEAT_TRAINER
#define NEAT_TRAINER

#include <neural/neat/Pool.h>
#include <neural/neat/Network.h>
#include <thread>
#include <mutex>
#include <sim/Simulator.h>
#include <neural/FileWriter.h>

class NeatTrainer
{
public:
    NeatTrainer();
    ~NeatTrainer();

    void run();

private:
    void evaluate(neat::Genome& genome, Simulator * sim);
    void evaluate_thread(Simulator * sim);
    void showBest();
    bool improved = false;
	int generation;
    neat::Pool * pool;
    neat::Network *bestNetwork;
	neat::Genome *bestGenome;

    std::mutex mtx;
    std::vector<neat::Genome*> active_genomes;
	std::string savePath;

	neural::FileWriter * fw;
};

#endif