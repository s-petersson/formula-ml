#include "Pool.h"
#include "Constants.h"
#include <core/util/Random.h>
#include <algorithm>
#include <iostream>
#include <unordered_map>

using namespace neat;
using namespace std;

Pool::Pool() {

    generation = 0;
    innovation = 0;
    currentSpecies = 1;
    currentGenome = 1;
    currentFrame = 0;
    maxFitness = 0;
}

Pool::~Pool() {

}
void Pool::fill() {
	for (int i = 0; i < Population; i++) {
		addToSpecies(basic_genome());
	}
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
        Species s = species[rngi((int)species.size())];
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
        auto remove = s.genomes.size() / 2; // Cut half the population. 
        if (cutToOne) remove = s.genomes.size() - 1;
        s.genomes.erase(s.genomes.end() - remove, s.genomes.end());
    }
}

void Pool::removeStaleSpecies() {
    vector<Species> survived;

    for (auto && s : species) {
        if (s.genomes.size() == 0) {
            cout << "Empty species" << endl;
            continue;
        }
        // Actually quite inefficient to actutally sort to find the max, better to just search.
        sort(s.genomes.begin(), s.genomes.end(), [](Genome a, Genome b) {return a.fitness > b.fitness; });
        if (s.genomes[0].fitness > s.topFitness) {
            s.topFitness = s.genomes[0].fitness;
            s.staleness = 0;
        }
        else {
            s.staleness++;
        }

        if (s.staleness < StaleSpecies || s.topFitness >= maxFitness) {
            survived.push_back(s);
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
    if (Config::InitialStructure) {
        for (int i = 0; i < Config::Inputs; ++i) {
            for (int o = 0; o < Config::Outputs; ++o) {
                Gene g(i+1, MaxNodes + o + 1, 0);
                g.weight = rngf() * 4.0f - 2.0f;
                genome.genes.push_back(g);
            }
        }
    }
    genome.maxneuron = Config::Inputs;
    genome.mutate();
    return genome;
}




float disjoint(vector<Gene> genes1, vector<Gene> genes2) {
	unordered_map<int, bool> i1, i2;

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
    int n = (int)glm::max(genes1.size(), genes2.size());
    return (float)disjointGenes / n;
}

float weights(vector<Gene> genes1, vector<Gene> genes2) {
	unordered_map<int, Gene> i2;
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