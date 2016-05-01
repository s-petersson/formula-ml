#ifndef SIMULATION_EVALUATOR
#define SIMULATION_EVALUATOR

#include <experiments/Experiment.h>
#include <neural/neat/Trainer.h>
#include <sim/Simulator.h>

struct AiSettings{
    // Inputs
    bool angle_to_line = false;
    bool distance_to_middle = false;
    bool distance_to_edges = false;
    bool speed = false;

    bool curve_data = false;
    int nbr_of_curve_points;
    float curve_point_spacing;
    float curve_point_spacing_incremental_percentage;

    bool curve_data_sum_absolutes = false;
    bool curve_data_flip = false;

    bool checkpoint_data = false;
    int checkpoint_data_nbr;

    // Outputs
    /// Steer is default
    bool output_speed = true;
};

struct SimulatorSettings {
    const string* track_path;
    bool completeTrack;
    float termination_distance;
    float max_time;

    float car_speed_min = 0;
    float car_speed_max;
    float car_speed_initial = 0;

	float min_avg_speed = 0;
	float avg_speed_excemption_distance = 0;
};

int required_nbr_of_inputs(const AiSettings& settings);
int required_nbr_of_outputs(const AiSettings& settings);
void set_neat_config(const AiSettings& settings);
void print_settings(const AiSettings& settings);


class SimulationEvaluator : public neat::Evaluator
{
public:
    SimulationEvaluator();
    ~SimulationEvaluator();
    void init();

    EvaluationResult evaluate_network(neat::Network* network);
    SimulationResult run(neat::Network* network);
    void reset();

    std::function<SimulationEvaluator*()> makeFactory();

    Simulator* getSimulator();
    neat::Network** getNetworkLocation();

    // Mandatory parameters
    SimulatorSettings sim_settings;
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