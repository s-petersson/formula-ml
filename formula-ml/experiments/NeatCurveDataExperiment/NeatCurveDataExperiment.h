#ifndef FORMULA_ML_NEATNETWORKMIDLINE_H
#define FORMULA_ML_NEATNETWORKMIDLINE_H


#include <sim/model/CarModel.h>
#include <functional>
#include <NeatTrainer.h>

#include <experiments/Experiment.h>
#include <experiments/ExperimentWindow.h>

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
    string load_network_path;

    NeatTrainer* trainer;
    ExperimentWindow* window;
};

#endif
