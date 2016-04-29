#ifndef NEAT_TRAINER
#define NEAT_TRAINER

#include <experiments/Experiment.h>
#include <neural/neat/Pool.h>
#include <neural/neat/Network.h>
#include <string>
#include <mutex>
#include <neural/FileWriter.h>

namespace neat {

	class Evaluator
	{
	public:
		virtual EvaluationResult evaluate_network(neat::Network* network) = 0; // also take memory ownage of network
	};

	class Trainer
	{
	public:
		Trainer();
		Trainer(std::string path);
		~Trainer();
		void run();

		neat::Genome get_best();
		void set_best(neat::Genome& genome, EvaluationResult evaluationResult);

		// Mandatory parameters
		std::function<Evaluator*()> evaluator_factory;

		// Optional parameters
		std::function<void(EvaluationResult evaluationResult)> on_new_best;
		std::function<void(int generation)> on_generation_done;

        std::string savePath;
	private:
		void evaluate(neat::Genome& genome, Evaluator* eval);
		void evaluate_thread(Evaluator* eval);
		
		bool improved = false;
		int generation;

		neat::Genome best_genome;
		std::mutex best_genome_mutex;

		std::mutex mtx;
		std::vector<neat::Genome*> active_genomes;
		
		neat::Pool * pool;
		neural::FileWriter * fw;
	};

}
#endif