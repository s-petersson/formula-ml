#ifndef NEURAL_EVOLVINGNETWORK
#define NEURAL_EVOLVINGNETWORK

#include "Neural.h"
#include <vector>

namespace neural {

	class EvolvingNetwork : public Network {
	public:
		EvolvingNetwork();
		~EvolvingNetwork();

		virtual NetworkIO fire(NetworkIO input);

	private:
		class Node {
		public:
			Node();
			~Node();

			float value;
			int out_edges_count;
			int in_edges_count;

			std::vector<Node*> in_edges;
			std::vector<Node*> out_edges;

			//Weights have the same index as the corresponding incoming edge.			
			std::vector<float> in_weights;
		};
		
		int node_count;
		int input_count;
		int output_count;

		std::vector<Node*> input_nodes;
		std::vector<Node*> output_nodes;
		std::vector<Node*> hidden_nodes;
	};
} // namespace neural 

#endif