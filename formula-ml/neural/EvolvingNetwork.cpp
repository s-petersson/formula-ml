#include "EvolvingNetwork.h"
#include <iostream>

using namespace neural;

EvolvingNetwork::EvolvingNetwork() {
	
	//Createing a network with two layers
	int input_count = 3;
	int output_count = 2;

	//Adding input nodes
	for (int i = 0; i < input_count; i++) {
		Node temp;
		nodes.push_back(temp);
		input_nodes_index.push_back(nodes.size() - 1);
	}

	//Adding output nodes
	for (int i = 0; i < output_count; i++) {
		Node temp;
		nodes.push_back(temp);
		output_nodes_index.push_back(nodes.size()-1);
	}

	/*
	//Connecting to all output edges
	for (int j = 0; j < output_count; j++) {
	temp.out_edges_index.push_back(output_nodes_index[j]);
	nodes[output_nodes_index[j]].in_edges_index.push_back(nodes.size());
	}
	*/
	addEdge(0, 3, 0.5);
	debug();
	addHiddenNode(0, 3);

	debug();
	int age;
	std::cin >> age;
};

EvolvingNetwork::~EvolvingNetwork() {

}
/*
Add a new node on an edge between two existing nodes.
*/
void EvolvingNetwork::addHiddenNode(int index_from, int index_to) {
	//There have to be an edge between the nodes to create a node inbetween.
	Node from = nodes[index_from];
	if (std::find(from.out_edges_index.begin(), from.out_edges_index.end(), index_to) == from.out_edges_index.end()){
		addEdge(index_from, index_to, 0.5);
	}
	

	Node to = nodes[index_to];
	Node temp;
	int index = nodes.size();

	nodes.push_back(temp);
	addEdge(index_from, index, 0.5);
	addEdge(index, index_to, 0.5);
	//Replacing the current


}

void EvolvingNetwork::addEdge(int index_from, int index_to, float weight) {

	//Must check so that a cyclic graph would not be created.
	nodes[index_from].out_edges_index.push_back(index_to);
	nodes[index_to].in_edges_index.push_back(index_from);

	//TODO Proper starting weights?
	nodes[index_to].in_weights.push_back(weight);
}

void EvolvingNetwork::removeEdge(int index_from, int index_to) {
	
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


