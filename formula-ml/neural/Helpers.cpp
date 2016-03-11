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
}