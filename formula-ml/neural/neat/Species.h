#ifndef NEURAL_NEAT_SPECIES
#define NEURAL_NEAT_SPECIES

#include <vector>
#include "Genome.h"

namespace neat {

    class Species {
    public:        
        Species();
        ~Species();

        Genome breedChild();
        void calculate_average_fitness();

        std::vector<Genome> genomes;
        float topFitness;
        float averageFitness;
        int staleness;
    };

}

#endif