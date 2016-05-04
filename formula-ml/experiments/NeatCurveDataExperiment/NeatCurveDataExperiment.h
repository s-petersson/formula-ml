#ifndef FORMULA_ML_NEATNETWORKMIDLINE_H
#define FORMULA_ML_NEATNETWORKMIDLINE_H

#include <sim/model/CarModel.h>
#include <functional>
#include <memory>

#include <experiments/Experiment.h>
#include <experiments/SimulationEvaluator.h>

#include <neural/neat/Trainer.h>
#include <neural/neat/Network.h>
#include <neural/neat/Constants.h>
#include <neural/Helpers.h>

#ifndef CLOUD_COMPUTING
#include <experiments/ExperimentWindow.h>
#endif

class NeatCurveDataExperiment : public Experiment {
public:
    NeatCurveDataExperiment();
	NeatCurveDataExperiment(string path);
    ~NeatCurveDataExperiment();
    virtual void run();	

    // Manditory
    AiSettings ai_settings;
    SimulatorSettings sim_settings;

private:
    string load_network_path;
    
    std::shared_ptr<neat::Trainer> trainer;

#ifndef CLOUD_COMPUTING
    std::shared_ptr<ExperimentWindow> window;
#endif
};

#endif
