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

		virtual void fire(const NetworkIO &input, NetworkIO &output);

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
		int createsCyclicGraph(int from_index, int to_index);
		int	isChild(std::vector<int> visited, int current_node_index, int target_index);
	};
} // namespace neural 

#endif