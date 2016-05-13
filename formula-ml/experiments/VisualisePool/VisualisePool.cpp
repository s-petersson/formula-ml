#include "VisualisePool.h"

VisualisePool::VisualisePool (string pool_path) {
    this->pool_path = pool_path;
    fileWriter = new neural::FileWriter("");
}

VisualisePool::~VisualisePool() { }

void VisualisePool::run() {
    Config::sigmoid = [](float x) {
        return -1.0f + 2.0f / (1.0f + glm::exp(-x));
    };

    // Prepare the experiment
    print_settings(ai_settings);
    set_neat_config(ai_settings);

    SimulationEvaluator experiment = SimulationEvaluator();
    experiment.sim_settings = sim_settings;
    experiment.ai_settings = ai_settings;
    experiment.init();

    std::function<SimulationEvaluator*()> factory = experiment.makeFactory();

    SimulationEvaluator *eval = new SimulationEvaluator();
    eval->sim_settings = sim_settings;
    eval->ai_settings = ai_settings;
    eval->init();

#ifndef CLOUD_COMPUTING
    // Prepare the window
    shared_ptr<RacelineLogger> raceline_logger = make_shared<RacelineLogger>(eval);
    SimulationEvaluator* windowEnvironment = factory();
    window = make_shared<VisualisationWindow>(windowEnvironment->getSimulator(), raceline_logger);
    raceline_logger->init();
#endif

    pool = fileWriter->poolFromSingleFile(this->pool_path);
    SimulationEvaluator * poolEvaluator = factory();
    Genome bestGenome = Genome();
    bestGenome.fitness = 0;
    for (auto && species : pool->species) {
        for (auto && genome : species.genomes) {
            if(genome.fitness > bestGenome.fitness) {
                bestGenome = genome;
            }
        }
    }

    delete pool;

    Network * bestNetwork = new Network(bestGenome.genes);
    window->setBestNetwork(bestNetwork); // Visualisation
    windowEnvironment->setNetwork(bestNetwork); // Car steering

    EvaluationResult r = poolEvaluator->evaluate_network(bestNetwork);
    printf("Evaluated fitness: %f", r.fitness);

    printf("Fitness: %f", bestGenome.fitness);

    stringstream ss;
    ss << "/Users/simon/Desktop/" << "/render" <<  ".png";

    RacelineLoggerJob job;
    job.genome = bestGenome;
    job.location = ss.str();

    raceline_logger->add_job(job);

    window->run();
    
    delete bestNetwork;
}