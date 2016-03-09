#ifndef NEURAL_NEAT_GENOME
#define NEURAL_NEAT_GENOME

#include <vector>
#include <map>
#include <string>

#include "Gene.h"
#include "Network.h"


namespace neat {

    class Genome {
    public:
        Genome();
		//Genome(string path);
        ~Genome();

        void mutate();

        std::vector<Gene> genes;
        float fitness = 0;
        float adjustedFitness = 0;

        std::map<std::string, float> mutationRates;

        //neat::Network network;
        int maxneuron = 0;
        int globalRank = 0;

        
    private:
        
        int randomNeuron(bool input);
        bool containsLink(Gene link);

        /** Mutates the weights of a genome, large chance that each weight will be slightly altered, small chance to reset*/
        void pointMutate();
        /**
        * Add an edge to the network, there are many improvements to be made here.
        * The evolution process should speed up if the function allways mutates, ie. only generate valid mutations.
        */
        void linkMutate(bool forceBias);

        /** Splits an edge into two. */
        void nodeMutate();

        void enableDisableMutate(bool enable);
    };

}

#endif