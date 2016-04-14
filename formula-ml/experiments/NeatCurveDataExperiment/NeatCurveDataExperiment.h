#ifndef FORMULA_ML_NEATNETWORKMIDLINE_H
#define FORMULA_ML_NEATNETWORKMIDLINE_H


#include <sim/model/CarModel.h>
#include <functional>
#include <NeatTrainer.h>

#include <experiments/Experiment.h>

#include <neural/neat/Network.h>
#include <neural/neat/Constants.h>
#include <neural/Helpers.h>


class NeatCurveDataExperiment : public Experiment {
public:
    NeatCurveDataExperiment();
	NeatCurveDataExperiment(string path);
    ~NeatCurveDataExperiment();
    virtual void run();	

    // Manditory
    static float termination_distance;
    static float max_time;
    static float car_speed;

    static int nbr_of_curve_points;
    static float curve_point_spacing;
    static float curve_point_spacing_incremental_percentage;

private:
	void visualise();

    string load_network_path;

    NeatTrainer* trainer;
};

class CurveEvaluator : public NeatEvaluator
{
public:
    CurveEvaluator();
    ~CurveEvaluator();
    virtual float evaluate_network(neat::Network* network);
    SimulationResult run(neat::Network* network);
    void reset(neat::Network* network);

    Simulator* getSimulator() { return simulator; };

private:
    neural::Network* network;
    Simulator* simulator;
    neural::NetworkIO network_indata = neural::NetworkIO();
    neural::NetworkIO network_output = neural::NetworkIO();
};

#endif
