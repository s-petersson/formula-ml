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

		bool set_input_weight(int from, int to, float value);
		bool set_hidden_weight(int from, int to, int layer, float value);
		bool set_output_weight(int from, int to, float value);

		float get_input_weight(int from, int to);
		float get_hidden_weight(int from, int to, int layer);
		float get_output_weight(int from, int to);

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