#ifndef NEURAL_FIXEDNETWORKTRAINER
#define NEURAL_FIXEDNETWORKTRAINER

#include "Neural.h"
#include "FixedNetwork.h"
#include <sim/model/TrackModel.h>
#include <sim/Simulator.h>
#include <glm/gtx/vector_angle.hpp>

struct FixedNetworkGenome {
    float genes[6];
};

namespace neural {

	class FixedNetworkTrainer {
	public:
		FixedNetworkTrainer();
		~FixedNetworkTrainer();
		void run();

        void buildNetwork(FixedNetworkGenome& genome);
        SimulationResult testGenome(FixedNetworkGenome& genome);
        void enumerate(FixedNetworkGenome& genome, int number_of_genes, int gene_steps);
        CarControl makeCarControl(NetworkIO output);
        void setLineData(NetworkIO* network_indata, int offset, Simulator* sim);

        SimulationResult runSimulation(Network* network, TrackModel* track);
        FixedNetwork* network;
        FixedNetworkGenome bestGenome;
        SimulationResult bestResult;
        TrackModel* track;
	};

} // namespace neural 

#endif