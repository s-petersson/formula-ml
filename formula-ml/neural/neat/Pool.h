#ifndef NEURAL_NEAT_POOL
#define NEURAL_NEAT_POOL

#include <vector>

#include "Species.h"

namespace neat {
    class Pool {
    public:
        Pool();
        ~Pool();

        void new_generation();

        std::vector<Species> species;
        int generation;
        int innovation;
        int currentSpecies;
        int currentGenome;
        int currentFrame;
        float maxFitness;

    private:
        Genome basic_genome();
        void cullSpecies(bool);
        void removeWeakSpecies();
        void addToSpecies(Genome child);
        void removeStaleSpecies();
        void rankGlobally();

        bool sameSpecies(const Genome& genome1, const Genome& genome2);

        float totalAverageFitness();

    };
}

#endif
