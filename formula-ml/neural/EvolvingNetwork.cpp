#include "EvolvingNetwork.h"


using namespace neural;

EvolvingNetwork::EvolvingNetwork() {
	//Createing a network with two layers
	
	input_count = 2;
	output_count = 1;

	//Adding output nodes
	for (int i = 0; i < output_count; i++) {
		Node* temp = new Node();
		output_nodes.push_back(temp);
	}
	
	//Adding input nodes
	for (int i = 0; i < input_count; i++) {
		Node* temp = new Node();

		//Connecting to all output edges
		for (int j = 0; j < output_count; j++) {
			temp->out_edges.push_back(output_nodes[j]);
		}
		input_nodes.push_back(temp);
	}


};

EvolvingNetwork::~EvolvingNetwork() {

}

NetworkIO EvolvingNetwork::fire(NetworkIO input) {
	
	
	
	return{0,0};
}

/*
std::string debug() {
	
}
*/

//TODO Are these neccesary?
EvolvingNetwork::Node::Node() {
	in_edges_count = 0;
	out_edges_count = 0;

}

EvolvingNetwork::Node::~Node() {}


