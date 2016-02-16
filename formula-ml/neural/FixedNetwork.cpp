#include "FixedNetwork.h"
#include <core/util/Random.h>
#include <iostream>

using namespace neural;
using namespace std;

FixedNetwork::FixedNetwork(int input_count, int output_count, int hidden_layers, int hidden_layer_height) {
    // Set dimension variables.
    input_size = input_count;
    output_size = output_count;
    hidden_layer_count = hidden_layers;
    hidden_layer_size = hidden_layer_height;

    // Allocate memory for values and weights.
    output_values = new float[output_size];
    input_weights = new float[input_size * hidden_layer_size];
    output_weights = new float[output_size * hidden_layer_size];
    hidden_values = new float[hidden_layer_count * hidden_layer_size];
    hidden_weights = new float[(hidden_layer_count-1) * hidden_layer_size * hidden_layer_size];

    // Initialize the weights of the network. 
    for (int i = 0; i < input_size * hidden_layer_size; ++i) {
        input_weights[i] = 1.0f;
    }
    for (int i = 0; i < output_size * hidden_layer_size; ++i) {
        output_weights[i] = 1.0f;
    }
    for (int i = 0; i < (hidden_layer_count - 1) * hidden_layer_size * hidden_layer_size; ++i) {
        hidden_weights[i] = 1.0f;
    }
}

FixedNetwork::~FixedNetwork() {
    if (input_weights)  delete[] input_weights;
    if (output_weights) delete[] output_weights;
    if (hidden_values)  delete[] hidden_values;
    if (hidden_weights) delete[] hidden_weights;
    if (output_values)  delete[] output_values;
}

NetworkIO FixedNetwork::fire(NetworkIO input) {

    // Add the input values into the first layer of the hidden layer
    for (int j = 0; j < hidden_layer_size; j++) {   // For each first layer node
        hidden_values[j] = 0;                       // Reset value. 
        for (int i = 0; i < input_size; i++) {      // For each input
            //Add the weighted inputs to the node.
            hidden_values[j] += input.values[i] * input_weights[(j * input_size) + i];
        }
        hidden_values[j] = sigmoid(hidden_values[j]); // Activation function;
    }

    // Calculate the rest of the hidden nodes. 
    for (int l = 1; l < hidden_layer_count; ++l) {
        for (int j = 0; j < hidden_layer_size; j++) {
            int current = l * hidden_layer_size + j;
            hidden_values[current] = 0; // Reset node.
            for (int i = 0; i < hidden_layer_size; i++) {
                int in = (l - 1) * hidden_layer_size + i;
                hidden_values[current] += hidden_values[in] * hidden_weights[(current * hidden_layer_size) + in];
            }
            hidden_values[current] = sigmoid(hidden_values[current]);
        }
    }
    // Calculate output layer
    for (int o = 0; o < output_size; ++o) {
        output_values[o] = 0.0f; // Reset node value. 
        int value_base = hidden_layer_size * (hidden_layer_count - 1);
        for (int h = 0; h < hidden_layer_size; ++h) {
            output_values[o] += hidden_values[value_base + h] * output_weights[(o * hidden_layer_size) + h];
        }
        output_values[o] = sigmoid(output_values[o]);
        
    }

    // Return struct pointing to ouput
    return {output_values, output_size};
}


int FixedNetwork::inputSize() {
    return input_size;
}

int FixedNetwork::outputSize() {
    return output_size;
}