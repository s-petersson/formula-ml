#include "NetworkView.h"
#include <iostream>
#include <neural/neat/Constants.h>

#include <core/util/Util.h>
using namespace neat;
using namespace gui;
using namespace std;
using namespace glm;

NetworkView::NetworkView(Network* net) {
    network = net;
    gui = new View;
    initialize();
}

NetworkView::~NetworkView() {
    if(gui) delete gui;
    positions.clear();
}

void NetworkView::render() {
	gui->render();
}

void NetworkView::update(float dt) {
    gui->clear();

    for (auto && e : edges) {
        vec3 delta = positions[e.out] - positions[e.in];
        auto len = length(delta);
        delta *= len * len * 0.0001f;
        if (length(delta) < 100) delta *= 0.f;
        if (e.in > network->inputSize()) {
            positions[e.in] += delta * dt;
        }
        if (e.out < MaxNodes) {
            positions[e.out] += -delta * dt;
        }
    }


    // Add the gui;
    static const float nr = 5.0f;
    for (auto && n : positions) {
        auto i = n.first;
        auto p = n.second;
        if (i > network->inputSize() && i < MaxNodes) {
            gui->add_circle(vec3(p.x - nr, p.y - nr, 0), vec3(p.x + nr, p.y + nr, 0), vec4(1.0f, 1.0f, 1.0f, 1.0f));
        }
        else {
            gui->add_rect(vec3(p.x - nr, p.y - nr, 0), vec3(p.x + nr, p.y + nr, 0), vec4(0.5f, 0.5f, 0.8f, 1.0f));
        }        
    }

    for (auto && e : edges) {
        auto p1 = positions[e.in];
        auto p2 = positions[e.out];

        if (e.in <= network->inputSize()) {
            p1 += vec3(nr, 0, 0);
        }
        if (e.out > MaxNodes) {
            p2 -= vec3(nr, 0, 0);
        }

        gui->add_line(p1, p2, vec4(1,1,1,0.5));
    }
}

void NetworkView::initialize() {

    vector<pair<int, Neuron>> inputs, hidden, outputs;
    for (auto && i : network->neurons) {
        if (i.first <= network->inputSize()) {
            inputs.push_back(i);
        }
        else if (i.first >= MaxNodes) {
            outputs.push_back(i);
        }
        else {
            hidden.push_back(i);
        }
        for (auto && e : i.second.incoming) {
            edges.push_back({e.into, e.out});
        }
    }

    float nr = 5.0f;
    int input_width = (int)glm::floor(glm::sqrt(network->inputSize())) + 1;
    float height = 50.0f + 4.0f * nr * (inputs.size() + 1);
    for (int i = 0; i < inputs.size(); i++) {
        //float y = 50 + 40 * (i / input_width) + (i % 2) * 20;
        //float x = 50 + 40 * (i % input_width);
        float y = 50.0f + 4.0f * nr * i;
        float x = 50.0f;
        auto p = inputs[i];
        positions[p.first] = vec3(x, y, 0);
    }

    for (int i = 0; i < hidden.size(); i++) {
        float x = input_width * 40.0f + 100.0f;
        float y = 50 + 4 * nr * i;
        auto p = hidden[i];
        positions[p.first] = vec3(x, y, 0);
    }
    float output_step = height / (outputs.size() + 1);
    for (int i = 0; i < outputs.size(); i++) {
        float x = input_width * 40.0f + 200.0f;
        float y = output_step * (i+1);
        auto p = outputs[i];
        positions[p.first] = vec3(x, y, 0);
    }

}

void NetworkView::reset() {

}
