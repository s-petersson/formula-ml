#ifndef NEURAL_FIXEDNETWORK
#define NEURAL_FIXEDNETWORK

#include "Neural.h"
#include <array>

namespace neural {

    class FixedNetwork : public Network {
    public:
        FixedNetwork(int input_count, int output_count, int hidden_layers, int hidden_layer_height);
        ~FixedNetwork();

        virtual NetworkIO fire(NetworkIO input);

        virtual int inputSize();
        virtual int outputSize();

    private:
        int input_size;
        int output_size;
        
        float * input_weights;
        float * output_weights;

        int hidden_layer_count;
        int hidden_layer_size;
        float * hidden_values;
        float * hidden_weights;

        float * output_values;
    };

} // namespace neural 

#endif