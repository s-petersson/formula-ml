#ifndef FORMULA_ML_NEATNETWORKMIDLINE_H
#define FORMULA_ML_NEATNETWORKMIDLINE_H


#include <sim/model/CarModel.h>
#include <functional>
#include <neural/neat/Trainer.h>

#include <experiments/Experiment.h>
#include <experiments/ExperimentWindow.h>
#include <experiments/SimulationEvaluator.h>

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
    float termination_distance;
    float max_time;
    float car_speed;

    AiSettings ai_settings;

private:
    string load_network_path;

    neat::Trainer* trainer;
    ExperimentWindow* window;
};

#endif
