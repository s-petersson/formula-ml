#include "EvolvingNetwork.h"
#include <iostream>

using namespace neural;

EvolvingNetwork::EvolvingNetwork() {
	
	//Createing a network with two layers
	int input_count = 1;
	int output_count = 1;

	//Adding input nodes
	for (int i = 0; i < input_count; i++) {
		Node temp;
		nodes.push_back(temp);
		input_nodes_index.push_back((int) nodes.size() - 1);
	}

	//Adding output nodes
	for (int i = 0; i < output_count; i++) {
		Node temp;
		nodes.push_back(temp);
		output_nodes_index.push_back((int) nodes.size()-1);
	}

	/*
	//Connecting to all output edges
	for (int j = 0; j < output_count; j++) {
	temp.out_edges_index.push_back(output_nodes_index[j]);
	nodes[output_nodes_index[j]].in_edges_index.push_back(nodes.size());
	}
	*/


	addEdge(0, 1, 0.5);

	debug();
	
	addHiddenNode(0, 1);
	addHiddenNode(2, 1);
	addHiddenNode(3, 1);
	debug();

	std::cout << "Creates acyclic(2, 4): " << createsCyclicGraph(2, 4) << std::endl;
	std::cout << "Creates acyclic(4, 2): " << createsCyclicGraph(4, 2) << std::endl;

	int stop;
	std::cin >> stop;
};

EvolvingNetwork::~EvolvingNetwork() {

}
/*
Add a new node on an edge between two existing nodes.
*/
void EvolvingNetwork::addHiddenNode(int index_from, int index_to) {
	//There have to be an edge between the nodes to create a node inbetween.
	Node from = nodes[index_from];
	/*
	if (std::find(from.out_edges_index.begin(), from.out_edges_index.end(), index_to) == from.out_edges_index.end()){
		addEdge(index_from, index_to, 0.5);
	}
	*/
	removeEdge(index_from, index_to);
	Node to = nodes[index_to];
	Node temp;
	int index = (int) nodes.size();

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
	Node *from = &(nodes[index_from]);
	Node *to = &(nodes[index_to]);

	//Remove pointers from
	for (int i = 0; i < (*from).out_edges_index.size(); i++) {
		if ((*from).out_edges_index[i] == index_to) {
			(*from).out_edges_index.erase((*from).out_edges_index.begin() + i);
		}
	}

	//Remove pointers and weights to
	for (int i = 0; i < (*to).in_edges_index.size(); i++) {
		if ((*to).in_edges_index[i] == index_from) {
			(*to).in_edges_index.erase((*to).in_edges_index.begin() + i);
			(*to).in_weights.erase((*to).in_weights.begin() + i);
		}
	}
}

/*
	Return 0 if acyclic, and 1 if a cyclic graph would be created
*/
int EvolvingNetwork::createsCyclicGraph(int from_index, int to_index) {

	if (from_index == to_index) {
		return 1;
	}
	std::vector<int> visited;
	//Recursion to check if the suggested parent is a child of the suggested child

	for (int i = 0; i < nodes.size(); i++) {
		visited.push_back(0);
	}
	visited[to_index] = 1;
	return isChild(visited, to_index, from_index);
}

/*
	returns 1 if target is a child, 0 otherwise.
*/
int EvolvingNetwork::isChild(std::vector<int> visited, int current_node_index, int target_index) {
	Node current_node = nodes[current_node_index];
	
	for (int i = 0; i < current_node.out_edges_index.size(); i++) {
		if (visited[current_node.out_edges_index[i]] == 0) {
			if (current_node.out_edges_index[i] == target_index) {
				return 1;
			}
			visited[current_node.out_edges_index[i]] = 1;
			return isChild(visited, current_node.out_edges_index[i], target_index);
		}
	}

	return 0;
}

int EvolvingNetwork::inputSize() {
	return (int) output_nodes_index.size();
}
int EvolvingNetwork::outputSize() {
	return (int) input_nodes_index.size();
}


void EvolvingNetwork::fire(const NetworkIO &input, NetworkIO &output) {
	
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


