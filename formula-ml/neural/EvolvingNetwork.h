#ifndef NEURAL_EVOLVINGNETWORK
#define NEURAL_EVOLVINGNETWORK

#include "Neural.h"
#include <vector>

namespace neural {

	class EvolvingNetwork : public Network {
	public:
		EvolvingNetwork();
		~EvolvingNetwork();

		virtual int inputSize();
		virtual int outputSize();

		virtual NetworkIO fire(NetworkIO input);

	private:
		class Node {
		public:
			Node();
			~Node();

			float value;

			std::vector<int> in_edges_index;
			std::vector<int> out_edges_index;
			//Weights have the same index as the corresponding incoming edge.			
			std::vector<float> in_weights;
		};

		std::vector<Node> nodes;
		std::vector<int> input_nodes_index;
		std::vector<int> output_nodes_index;
		std::vector<int> hidden_nodes_index;
		
		void debug();
		void addHiddenNode(int index_from, int index_to);
		void addEdge(int index_from, int index_to, float weight);
		void removeEdge(int index_from, int index_to);
	};
} // namespace neural 

#endif