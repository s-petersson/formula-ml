#ifndef NEURAL_NEURAL
#define NEURAL_NEURAL
#include <glm/glm.hpp>

namespace neural {

    struct NetworkIO {
        float * values;
        int value_count;
    };

    class Network {
    public:
        virtual NetworkIO fire(NetworkIO input) = 0;

        virtual int inputSize() = 0;
        virtual int outputSize() = 0;
    };

    inline float sigmoid(float x) {
        return 1.0f / (1.0f + glm::exp(-x));
    }

} // namespace neural

#endif