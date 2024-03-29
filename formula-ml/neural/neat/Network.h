#ifndef NEURAL_NEAT_NETWORK
#define NEURAL_NEAT_NETWORK
#include <neural/Neural.h>
#include "Gene.h"

#include <vector>
#include <map>

namespace neat {
    
    class Neuron {
    public:
        Neuron();
        Neuron(const Neuron &neuron);
        ~Neuron();

        std::vector<Gene> incoming;
        float value = 0.0f;
        bool created = false;

    };

    class Network : public neural::Network{
    public:
        Network();
        Network(std::vector<Gene> genes);
        ~Network();

        void reset();
        void fire(const neural::NetworkIO &input, neural::NetworkIO &output);

        int inputSize();
        int outputSize();
		std::map<int, Neuron> neurons;
    private:
        void evaluateNetwork(float* inputs, int input_count, float* outputs, int output_count);
        
        const int Inputs;
        const int Outputs;

    };

}

#endif
