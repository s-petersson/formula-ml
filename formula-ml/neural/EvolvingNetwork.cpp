#include "EvolvingNetwork.h"
#include <iostream>

using namespace neural;

EvolvingNetwork::EvolvingNetwork() {
	
	//Createing a network with two layers
	int input_count = 3;
	int output_count = 2;

	//Adding output nodes
	for (int i = 0; i < output_count; i++) {
		Node temp;
		nodes.push_back(temp);
		output_nodes_index.push_back(nodes.size()-1);
	}
	
	//Adding input nodes
	for (int i = 0; i < input_count; i++) {
		Node temp;

		//Connecting to all output edges
		for (int j = 0; j < output_count; j++) {
			temp.out_edges_index.push_back(output_nodes_index[j]);
			nodes[output_nodes_index[j]].in_edges_index.push_back(nodes.size());
		}
		nodes.push_back(temp);
		input_nodes_index.push_back(nodes.size() - 1);
	}
	debug();
	int age;
	std::cin >> age;
};

EvolvingNetwork::~EvolvingNetwork() {

}

int EvolvingNetwork::inputSize() {
	return output_nodes_index.size();
}
int EvolvingNetwork::outputSize() {
	return input_nodes_index.size();
}


NetworkIO EvolvingNetwork::fire(NetworkIO input) {
	
	
	
	return{0,0};
}


void EvolvingNetwork::debug() {
	std::cout << nodes.size() << std::endl;
	for (int i = 0; i < nodes.size(); i++) {
		std::cout << "Node " << i << ":" << std::endl;
		std::cout << "Input Nodes: ";
		for (int j = 0; j < nodes[i].in_edges_index.size(); j++) {
			std::cout << nodes[i].in_edges_index[j] << " ";
		}
		std::cout << std::endl << "Output Nodes: ";
		for (int j = 0; j < nodes[i].out_edges_index.size(); j++) {
			std::cout << nodes[i].out_edges_index[j] << " ";
		}
		std::cout << std::endl << std::endl;

	}
}


//TODO Are these neccesary?
EvolvingNetwork::Node::Node() {
	
}

EvolvingNetwork::Node::~Node() {}


