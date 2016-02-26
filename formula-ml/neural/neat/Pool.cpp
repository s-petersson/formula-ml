#include "Pool.h"
#include "Constants.h"
#include <core/util/Random.h>
#include <algorithm>
#include <iostream>

using namespace neat;
using namespace std;

Pool::Pool() {
    generation = 0;
    innovation = 0;
    currentSpecies = 1;
    currentGenome = 1;
    currentFrame = 0;
    maxFitness = 0;

    for (int i = 0; i < Population; i++) {
        addToSpecies(basic_genome());
    }
    
}

Pool::~Pool() {

}

void Pool::new_generation() {
    cullSpecies(false);
    rankGlobally();
    removeStaleSpecies();
    rankGlobally();
    for (auto && s : species) {
        s.calculate_average_fitness();
    }
    removeWeakSpecies();

    float sum = totalAverageFitness();
    vector<Genome> children;
    for (auto && s : species) {
        int breed = (int)glm::floor(s.averageFitness / sum * Population) - 1; // What is this for?
        for (int i = 0; i < breed; i++) {
            children.push_back(s.breedChild());
        }
    }
    cullSpecies(true);
    while (children.size() + species.size() < Population) {
        Species s = species[rngi(species.size())];
        children.push_back(s.breedChild());
    }
    for (auto && i : children) {
        addToSpecies(i);
    }
    generation++;

    // Write state to file to backup!!!!!  
}

void Pool::cullSpecies(bool cutToOne) {
    for (auto && s : species) {
        sort(s.genomes.begin(), s.genomes.end(), [](Genome a, Genome b) {return a.fitness > b.fitness; });
        int remove = s.genomes.size() / 2; // Cut half the population. 
        if (cutToOne) remove = s.genomes.size() - 1;
        s.genomes.erase(s.genomes.end() - remove, s.genomes.end());
    }
}

void Pool::removeStaleSpecies() {
    vector<Species> survived;

    for (auto && species : species) {
        if (species.genomes.size() == 0) {
            cout << "Empty species" << endl;
            continue;
        }
        // Actually quite inefficient to actutally sort to find the max, better to just search.
        sort(species.genomes.begin(), species.genomes.end(), [](Genome a, Genome b) {return a.fitness > b.fitness; });
        if (species.genomes[0].fitness > species.topFitness) {
            species.topFitness = species.genomes[0].fitness;
            species.staleness = 0;
        }
        else {
            species.staleness++;
        }

        if (species.staleness < StaleSpecies || species.topFitness >= maxFitness) {
            survived.push_back(species);
        }
    }
    species = survived;
}

void Pool::removeWeakSpecies() {
    vector<Species> survived;

    float sum = totalAverageFitness();
    for (int i = 0; i < species.size(); i++) {
        Species s = species[i];
        // Actually quite inefficient to actutally sort to find the max, better to just search.
        int breed = (int)glm::floor(s.averageFitness / sum * Population); // This is just weird, test whether this actually works.
        if (breed >= 1) {
            survived.push_back(s);
        }
    }
    species = survived;
}

void Pool::addToSpecies(Genome child) {
    bool foundSpecies = false;
    for (auto && s : species) {
        if (sameSpecies(child, s.genomes[0])) {
            //cout << "Found species!" << endl;
            s.genomes.push_back(child);
            foundSpecies = true;
            break;
        }
    }

    if (!foundSpecies) {
        Species childSpecies;
        childSpecies.genomes.push_back(child);
        species.push_back(childSpecies);
    }
}

Genome Pool::basic_genome() {
    Genome genome;
    genome.maxneuron = Inputs;
    genome.mutate();
    return genome;
}




float disjoint(vector<Gene> genes1, vector<Gene> genes2) {
    map<int, bool> i1, i2;

    for (auto && gene : genes1) {
        i1[gene.innovation] = true;
    }
    for (auto && gene : genes2) {
        i2[gene.innovation] = true;
    }
    int disjointGenes = 0;

    for (auto && gene : genes1) {
        if (!i2[gene.innovation]) disjointGenes++;
    }


    for (auto && gene : genes2) {
        if (!i1[gene.innovation]) disjointGenes++;
    }
    int n = glm::max(genes1.size(), genes2.size());
    return (float)disjointGenes / n;
}

float weights(vector<Gene> genes1, vector<Gene> genes2) {
    map<int, Gene> i2;
    for (auto && gene : genes2) {
        i2[gene.innovation] = gene;
    }
    float sum = 0.0f;
    float coincident = 0.0f;

    for (auto && gene : genes1) {
        Gene gene2 = i2[gene.innovation];
        if (gene2.created) {
            sum += glm::abs(gene.weight - gene2.weight);
            coincident += 1.0f;
        }
    }

    return sum / coincident;
}

bool Pool::sameSpecies(const Genome& genome1, const Genome& genome2) {
    float dd = DeltaDisjoint*disjoint(genome1.genes, genome2.genes);
    float dw = DeltaWeights*weights(genome1.genes, genome2.genes);
    return (dd + dw) < DeltaThreshold;
}

// Assigns each genome its global rank. 
void Pool::rankGlobally() {
    vector<Genome*> global;
    for (auto && s : species) {
        for (int i = 0; i < s.genomes.size(); i++) {
            global.push_back(&s.genomes[i]);
        }
    }
    sort(global.begin(), global.end(), [](Genome* g1, Genome* g2) {return g1->fitness < g2->fitness; });

    for (int i = 0; i < global.size(); i++) {
        global[i]->globalRank = i;
    }
}

// Why not just averagefitness?
float Pool::totalAverageFitness() {
    float total = 0.0f;
    for (const auto & s : species) {
        total += s.averageFitness;
    }
    return total;
}