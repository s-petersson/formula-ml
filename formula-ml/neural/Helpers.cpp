#include <neural/Helpers.h>
#include <glm/glm.hpp>

namespace neural{
    float sum(float* data, int size) {
        float result = 0;
        for (int i = 0; i < size; i++) {
            result += data[i];
        }
        return result;
    }
    float sum_absolutes(float* data, int size) {
        float result = 0;
        for (int i = 0; i < size; i++) {
            result += glm::abs(data[i]);
        }
        return result;
    }

    void flip_parity(float* target, int size) {
        for (int i = 0; i < size; i++) {
            target[i] *= -1;
        }
    }
    void flip_parity_if(float* target, int size, bool predicate) {
        if (predicate) {
            flip_parity(target, size);
        }
    }

    float fitness_distance_time(SimulationResult result, float termination_distance, float max_time) {
        float fitness = result.distance_driven;

        // If the car has come to the end of the track, let the time
        if (glm::abs(result.distance_driven - termination_distance) < 1) {
            // (sqrt(t_max) - sqrt(t))^2 gives increases the fitness for lower times with
            // higher resolution at lower values. *5 stretches the result to equal the
            // size of distance somewhat.
            float root_difference = glm::sqrt(max_time) - glm::sqrt(result.time_alive);
            float time_fitness = root_difference * root_difference * 5.f;

            // If the car would drive slower than the maximum_time constant, make the time a penalty instead
            fitness += result.time_alive < max_time ? time_fitness : -time_fitness;
        }

        return fitness;
    }
}