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
    ~NeatCurveDataExperiment();
    virtual void run();
    virtual std::function<CarControl()> updater();

	

    // Manditory
    static int nbr_of_curve_points;
    static float termination_distance;
    static float max_time;

private:
	void visualise();

    NeatTrainer* trainer;
};

class CurveEvaluator : public NeatEvaluator
{
public:
    CurveEvaluator();
    ~CurveEvaluator();
    virtual float evaluate_network(neural::Network* network);
    SimulationResult run(neural::Network* network);
    void reset(neural::Network* network);

    Simulator* getSimulator() { return simulator; };

private:
    neural::Network* network;
    Simulator* simulator;
    neural::NetworkIO network_indata = neural::NetworkIO();
    neural::NetworkIO network_output = neural::NetworkIO();
};

#endif
