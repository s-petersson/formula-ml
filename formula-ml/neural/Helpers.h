#ifndef NEURAL_HELPERS
#define NEURAL_HELPERS

namespace neural {
    float sum(float* data, int size);

    float sum_absolutes(float* data, int size);

    void flip_parity(float* target, int size);

    void flip_parity_if(float* target, int size, bool predicate);
}

#endif