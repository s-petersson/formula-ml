#include "NetworkView.h"
#include <iostream>
using namespace neat;
using namespace gui;
using namespace std;
using namespace glm;

NetworkView::NetworkView(Network* net) {
	network = net;
    gui = new View;
	vector<pair<int, Neuron>> inputs, hidden, outputs;
	for (auto && i : network->neurons) {
		if (i.first <= network->inputSize()) {
			inputs.push_back(i);
		}
		else if (i.first >= 100000) {
			outputs.push_back(i);
		}
		else {
			hidden.push_back(i);
		}
		
		
	}
	int input_width = glm::floor(glm::sqrt(network->inputSize())) + 1;
	for (int i = 0; i < inputs.size(); i++) {
		float y = 50 + 40 * (i / input_width);
		float x = 50 + 40 * (i % input_width);
		gui->add_rect(vec3(x - 15, y - 15, 0), vec3(x + 15, y + 15, 0), vec4(1.0f, 1.0f, 1.0f, 0.5f));
		auto p = inputs[i];
		positions[p.first] = vec3(x, y, 0);
	}

	for (int i = 0; i < hidden.size(); i++) {
		float x = input_width * 40 + 100;
		float y = 50 + 40 * i;
		gui->add_rect(vec3(x - 15, y - 15, 0), vec3(x + 15, y + 15, 0), vec4(1.0f, 1.0f, 1.0f, 0.5f));
		auto p = hidden[i];
		positions[p.first] = vec3(x, y, 0);
	}

	for (int i = 0; i < outputs.size(); i++) {
		float x = input_width * 40 + 200;
		float y = 50 + 40 * i;
		gui->add_rect(vec3(x - 15, y - 15, 0), vec3(x + 15, y + 15, 0), vec4(1.0f, 1.0f, 1.0f, 0.5f));
		auto p = outputs[i];
		positions[p.first] = vec3(x, y, 0);
	}
	for (auto && p : network->neurons) {
		for (auto && g : p.second.incoming) {
			gui->add_line(positions[g.into] + vec3(15,0,0), positions[g.out] - vec3(15, 0, 0), vec4(0,1,0,1));
		}
	}

}

NetworkView::~NetworkView() {
    if(gui) delete gui;
    positions.clear();
}

void NetworkView::render() {
	gui->render();
}

void NetworkView::initialize() {

}