#ifndef FORMULA_ML_NEATNETWORKMIDLINE_H
#define FORMULA_ML_NEATNETWORKMIDLINE_H


#include <sim/model/CarModel.h>
#include <functional>
#include <NeatTrainer.h>
#include "Experiment.h"

class NEATNetworkMidline : public Experiment {
public:
    NEATNetworkMidline();
    ~NEATNetworkMidline();

    virtual void run();
    virtual std::function<CarControl()> updater();
private:
    NeatTrainer* trainer;
};

#endif
