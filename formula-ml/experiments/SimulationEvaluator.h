#ifndef SIMULATION_EVALUATOR
#define SIMULATION_EVALUATOR

#include <NeatTrainer.h>

struct AiSettings{
    bool angle_to_line;
    bool distance_to_middle;
    bool speed;

    bool curve_data;
        int nbr_of_curve_points;
        float curve_point_spacing;
        float curve_point_spacing_incremental_percentage;

        bool curve_data_sum_absolutes;
        bool curve_data_flip;
};

int required_nbr_of_inputs(const AiSettings& settings);
int required_nbr_of_outputs(const AiSettings& settings);
void set_neat_config(const AiSettings& settings);
void print_settings(const AiSettings& settings);


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

    Simulator* getSimulator();
    neat::Network** getNetworkLocation();

    // Manditory parameters
    float termination_distance;
    float max_time;
    float car_speed;

    AiSettings ai_settings;

private:
    void appendIf(bool predicate, float value);
    int input_iterator;

    Simulator* simulator;   // Memory owned by SimulationEvaluator
    neat::Network* network; // Memory not owned!

    neural::NetworkIO network_indata = neural::NetworkIO();
    neural::NetworkIO network_output = neural::NetworkIO();
};

#endif