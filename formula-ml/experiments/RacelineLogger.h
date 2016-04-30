#ifndef EXPERIMENTS_RACELINELOGGER
#define EXPERIMENTS_RACELINELOGGER

#include <string>
#include <memory>
#include <mutex>

#include <sim/Simulator.h>
#include <experiments/SimulationEvaluator.h>
#include <neural/neat/Genome.h>
#include "RacelineRenderer.h"
#include <core/gfx/Framebuffer.h>


struct RacelineLoggerJob {
    neat::Genome genome;
	std::string location;
};

class RacelineLogger {
public:
    RacelineLogger(SimulationEvaluator* evaluator);
    ~RacelineLogger();

    void init();

    void add_job(RacelineLoggerJob task);
    void process_jobs();

private:
    void save_raceline(const std::string& loc);

    std::mutex work_mutex;
    std::vector<RacelineLoggerJob> job_queue;
    SimulationEvaluator* evaluator;
    std::unique_ptr<RacelineRenderer> renderer;

    gfx::Framebuffer ms_fbo;
    gfx::Framebuffer final_fbo;
};

#endif