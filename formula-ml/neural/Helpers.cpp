#include <neural/Helpers.h>
#include <glm/glm.hpp>
#include <iostream>

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

	void write_segment_sums(float* data, int size, float* target, int size_target) {
		if (size != 10 || size_target != 3) {
			cout << "Helpers::write_sum(): Though shall not use this for other than 10 and 3\n";
			return;
		}

		for (int i = 0; i < size_target; i++) {
			target[i] = 0;
		}
		for (int i = 0; i <= 4; i++) {
			target[0] += data[i];
		}
		for (int i = 4; i <= 7; i++) {
			target[1] += data[i];
		}
		for (int i = 7; i <= 10; i++) {
			target[2] += data[i];
		}
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
        // Yield fitness liniarly for distance travelled
        float fitness = result.distance_on_track;

        // If the car reached the end, consider time
        if (glm::abs(result.distance_on_track - termination_distance) < 1) {
            // (sqrt(t_max) - sqrt(t))^2 gives increases the fitness for lower times with
            // higher resolution at lower values. *5 stretches the result to equal the
            // size of distance somewhat.
            float root_difference = glm::sqrt(max_time) - glm::sqrt(result.time_alive);
            float time_fitness = root_difference * root_difference * 5.f;

            // If the car would drive slower than the maximum_time constant, make the time a penalty instead
            fitness += result.time_alive < max_time ? time_fitness : 0;
        }

        return fitness;
    }

    float fitness_distance_time_exp(SimulationResult result, float termination_distance, float max_time, float min_time) {
        // Yield fitness liniarly for distance travelled
        // 100 for full completion
        float fitness = 100.f * result.distance_on_track / termination_distance;

        // If the car reached the end, consider time
        if (glm::abs(result.distance_on_track - termination_distance) < 1) {
            // 0    for infinite time
            // 10   for time "max_time"
            // 100  for time "min_time"
            // more for shorter time
            fitness += 100.f * glm::pow(0.1f, (result.time_alive - min_time) / (max_time - min_time));
        }

        return fitness;
    }
}