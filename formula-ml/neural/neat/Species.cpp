#include "Species.h"

#include <core/util/Random.h>
#include <unordered_map>


#include "Constants.h"

using namespace neat;
using namespace std;

Genome crossover(Genome g1, Genome g2) {
    if (g2.fitness > g1.fitness) {
        Genome temp = g1;
        g1 = g2;
        g2 = temp;
    }
    Genome child;
	unordered_map<int, Gene> innovations2;
    for (auto && i : g2.genes) {
        innovations2[i.innovation] = i;
    }

    for (int i = 1; i < g1.genes.size(); i++) {
        Gene gene1 = g1.genes[i];
        Gene gene2 = innovations2[gene1.innovation];

        // STRUCT NULL CHECK 
        if (gene2.enabled && rngf() > 0.5f) {
            child.genes.push_back(gene2); // If we change to pointers make sure to copy gene2 instead
        }
        else {
            child.genes.push_back(gene1);
        }
    }
    child.maxneuron = glm::max(g1.maxneuron, g2.maxneuron);

    // Copy mutation rates
    for (auto && i : g1.mutationRates) {
        child.mutationRates[i.first] = i.second;
    }

    return child;
}



Species::Species() {
    topFitness = 0;
    staleness = 0;
    averageFitness = 0;
}

Species::~Species() {

}

void Species::calculate_average_fitness() {
    int total = 0;

    for (const auto & genome : genomes) {
        total += genome.globalRank;
    }
    averageFitness = (float)total / genomes.size();
}

Genome Species::breedChild() {
    Genome child;
    if (rngf() < CrossoverChance) {
        Genome g1 = genomes[rngi((int)genomes.size())];
        Genome g2 = genomes[rngi((int)genomes.size())];
        child = crossover(g1, g2);
    }
    else {
        child = genomes[rngi((int)genomes.size())];
    }
    child.mutate();
    return child;
}
