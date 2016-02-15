#ifndef NEURAL_EVOLVINGNETWORK
#define NEURAL_EVOLVINGNETWORK

#include "Neural.h"

namespace neural {

	class EvolvingNetwork : public Network {
	public:
		EvolvingNetwork();
		~EvolvingNetwork();

		virtual NetworkIO fire(NetworkIO input);

	private:
		int nbrOfNodes;
	};

	class Node {
	public:
		float value;
		int out_edges_count;
		int in_edges_count;

		Node * out_edges;
		Node * in_edges;

		/**
		* Weights have the same index as the corresponding incoming edge. 
		*/

		float * in_weights;
	};

} // namespace neural 

#endif