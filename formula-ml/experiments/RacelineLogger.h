#ifndef EXPERIMENTS_RACELINELOGGER
#define EXPERIMENTS_RACELINELOGGER

#include <string>
#include <memory>
#include <mutex>

#include <sim/Simulator.h>
#include <experiments/SimulationEvaluator.h>
#include <neural/neat/Genome.h>

struct RacelineLoggerJob {
	
	std::shared_ptr<SimulationEvaluator> evaluator;
	std::string location;
};

class RacelineLogger {
public:
	void add_job(RacelineLoggerJob task);
	void process_jobs();

private:
	std::mutex work_mutex;
	std::vector<RacelineLoggerJob> job_queue;
};


#endif