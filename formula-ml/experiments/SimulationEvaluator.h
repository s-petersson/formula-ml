#ifndef SIMULATION_EVALUATOR
#define SIMULATION_EVALUATOR

#include <NeatTrainer.h>

class SimulationEvaluator : public NeatEvaluator
{
public:
    SimulationEvaluator();
    ~SimulationEvaluator();
    void init();

    float evaluate_network(neat::Network* network);
    SimulationResult run(neat::Network* network);
    void reset();

    std::function<SimulationEvaluator*()> makeFactory();

    Simulator* simulator;
    neat::Network* network; // Memory responsibility?

    // Manditory
    float termination_distance;
    float max_time;
    float car_speed;

    int nbr_of_curve_points;
    float curve_point_spacing;
    float curve_point_spacing_incremental_percentage;

private:
    neural::NetworkIO network_indata = neural::NetworkIO();
    neural::NetworkIO network_output = neural::NetworkIO();
};

#endif