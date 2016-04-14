#include "Network.h"
#include "Constants.h"
#include <algorithm>
#include <iostream>

using namespace neat;
using namespace std;

// Neuron Implementation
Neuron::Neuron() {
    value = 0.0f;
    created = true;
}

Neuron::Neuron(const Neuron &neuron) {
    value = neuron.value;
    created = neuron.created;
    incoming = neuron.incoming;
}

Neuron::~Neuron(){}


/* Network                                                          */
/*==================================================================*/

Network::Network() : Inputs(Config::Inputs), Outputs(Config::Outputs) {}

Network::Network(std::vector<Gene> genes) : Network(){
    neurons[0] = Neuron();
    neurons[0].value = 1.0f;
    for (int i = 1; i <= Inputs; i++) {
        neurons[i] = Neuron();
    }
    for (int i = 1; i <= Outputs; i++) {
        neurons[MaxNodes + i] = Neuron();
    }

    sort(genes.begin(), genes.end(), [](Gene a, Gene b) {return a.out < b.out; });

    for (int i = 0; i < genes.size(); i++) {
        Gene g = genes[i];
        if (g.enabled) {
            /*
            if (network.neurons[g.out].incoming.size() == 0) { // 0 check will probably be iffy, maybe use pointers or flags instead? Or maybe set memory values.
            network.neurons[g.out] = createNeuron();
            }*/
            if (!neurons[g.out].created) {
                neurons[g.out] = Neuron();
            }

            neurons[g.out].incoming.push_back(g);

            if (!neurons[g.into].created) {
                neurons[g.into] = Neuron();
            }
        }
    }
}

Network::~Network() {}

void Network::reset() {
    typedef std::map<int, Neuron>::iterator it_type;
    for (it_type iterator = neurons.begin(); iterator != neurons.end(); iterator++) {
        iterator->second.value = 0.0f;
    }
    neurons[0].value = 1.0f;
}

void Network::fire(const neural::NetworkIO &input, neural::NetworkIO &output) {
    evaluateNetwork(input.values, input.value_count, output.values, output.value_count);
}

int Network::inputSize() {
    return Inputs;
}

int Network::outputSize() {
    return Outputs;
}


/** Fills the array of outputs by computing the network with the provided inputs. */
void Network::evaluateNetwork(float* inputs, int input_count, float* outputs, int output_count) {
    /*for (auto && n : neurons) {
        n.second.value = 0.0f;
    }*/

    if (input_count != Inputs) {
        cout << "Incorrect number of inputs." << endl;
        return;
    }

    if (output_count != Outputs) {
        cout << "Incorrect number of outputs." << endl;
        return;
    }

    // Fill the input nodes with values.
    for (int i = 0; i < Inputs; i++) {
        neurons[i + 1].value = inputs[i];
    }
    // Calculate all the nodes, the sorting of the genes should!!! make this work.
    for (auto && node : neurons) {
        for (auto && g : node.second.incoming) {
            node.second.value += g.weight * neurons[g.into].value;
        }
        // The "original" uses a > 0 check here before using the sigmoid function.
        node.second.value = Config::sigmoid(node.second.value);
    }

    for (int i = 0; i < Outputs; i++) {
        outputs[i] = neurons[MaxNodes + i + 1].value;
    }
}