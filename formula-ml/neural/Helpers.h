#ifndef NEURAL_HELPERS
#define NEURAL_HELPERS

#include <sim/Simulator.h>

namespace neural {
    float sum(float* data, int size);
    float sum_absolutes(float* data, int size);

    void flip_parity(float* target, int size);
    void flip_parity_if(float* target, int size, bool predicate);

    float fitness_distance_time(SimulationResult result, float termination_distance, float max_time);
}

#endif