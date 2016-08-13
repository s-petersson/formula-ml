#include "Genome.h"
#include "Constants.h"
#include <core/util/Random.h>
#include <unordered_set>
#include <fstream>

#include <iostream>


using namespace neat;
using namespace std;

int innovation = 0;

Genome::Genome() {
    fitness = 0.0f;
    adjustedFitness = 0.0f;
    mutationRates["connections"] = MutateConnectionsChance;
    mutationRates["link"] = LinkMutationChance;
    mutationRates["bias"] = BiasMutationChance;
    mutationRates["node"] = NodeMutationChance;
    mutationRates["enable"] = EnableMutationChance;
    mutationRates["disable"] = DisableMutationChance;
    mutationRates["step"] = StepSize;

    originalMutationRates["connections"] = MutateConnectionsChance;
    originalMutationRates["link"] = LinkMutationChance;
    originalMutationRates["bias"] = BiasMutationChance;
    originalMutationRates["node"] = NodeMutationChance;
    originalMutationRates["enable"] = EnableMutationChance;
    originalMutationRates["disable"] = DisableMutationChance;
    originalMutationRates["step"] = StepSize;

}

Genome::~Genome() {}

int new_innovation() {
    return ++innovation;
}



/** Returns the index of a random neuron within a network. Input neurons will only be returned if input = true.*/
int Genome::randomNeuron(bool input) {
	unordered_set<int> neurons;

    if (input) {
        for (int i = 1; i <= Config::Inputs; i++) {
            neurons.insert(i);
        }
    } else {
        for (int i = 1; i <= Config::Outputs; i++) {
            neurons.insert(MaxNodes + i);
        }
    }

    for (auto && gene : genes) {
        if (gene.out < MaxNodes) {
            neurons.insert(gene.out);
        }
    }
    if (neurons.count(-1) > 0) cout << "-1 NODE IN GENOME" << endl;
    int temp = rngi((int)neurons.size());
    return *std::next(neurons.begin(), temp);;
}

bool Genome::containsLink(Gene link) {
    for (auto && gene : genes) {
        if (gene.into == link.into && gene.out == link.out) return true;
    }
    return false;
}

/** Mutates the weights of a genome, large chance that each weight will be slightly altered, small chance to reset*/
void Genome::pointMutate() {
    float step = mutationRates["step"];
    for (auto && gene : genes) {
        if (rngf() < PerturbChance) {
            gene.weight = gene.weight + rngf() * step * 2.0f - step;
        }
        else {
            gene.weight = rngf() * 4.0f - 2.0f;
        }
    }
}

/**
* Add an edge to the network, there are many improvements to be made here.
* The evolution process should speed up if the function allways mutates, ie. only generate valid mutations.
*/
void Genome::linkMutate(bool forceBias) {
    int neuron1 = randomNeuron(true);
    int neuron2 = randomNeuron(false);

    // No links between input nodes.
    if (neuron1 <= Config::Inputs && neuron2 <= Config::Inputs) {
        return;
    }

    if (forceBias) {
        //new_link.into = Config::Inputs; // Changed the Bias node index to index 0.
        neuron1 = 0;
    }
    Gene new_link(neuron1, neuron2, new_innovation());
    new_link.weight = rngf() * 4.0f - 2.0f;

    // Dont add duplicates
    if (containsLink(new_link)) return;

    genes.push_back(new_link);
}

/** Splits an edge into two. */
void Genome::nodeMutate() {
    if (genes.size() == 0) return;
    maxneuron++;

    int geneNumber = rngi((int)genes.size());
    Gene gene = genes[geneNumber];
    if (!gene.enabled) return;
    // disable the old link
    genes[geneNumber].enabled = false;

    // Copy the genes into two new genes.
    Gene gene1 = Gene(gene.into, maxneuron, new_innovation());
    Gene gene2 = Gene(maxneuron, gene.out, new_innovation());

    gene1.weight = 1.0f;
    genes.push_back(gene1);

    gene2.weight = gene.weight;
    genes.push_back(gene2);
}

void Genome::enableDisableMutate( bool enable) {
    vector<Gene*> candidates;
    for (int i = 0; i < genes.size(); i++) {
        Gene * gene = &genes[i];
        if (gene->enabled != enable) candidates.push_back(gene);
    }
    if (candidates.size() == 0) return;

    Gene* gene = candidates[rngi((int) candidates.size())];
    gene->enabled = !gene->enabled;
}

/** Does all the mutation stuff */
void Genome::mutate() {
    for (auto && i : mutationRates) {
        if (i.second >= originalMutationRates[i.first] * 0.5f) {
            if (rngi(2) == 1) {
                i.second *= 0.95f;
            } else {
                i.second *= 1.05263f; // Magic numbers?
            }
        }
    }

    if (rngf() < mutationRates["connections"]) {
        pointMutate();
    }
    {
        float p = mutationRates["link"];
        while (p > 0.0f) {
            if (rngf() < p) {
                linkMutate(false);
            }
            p -= 1.0f;
        }
    }
    {
        float p = mutationRates["bias"];
        while (p > 0.0f) {
            if (rngf() < p) {
                linkMutate(true);
            }
            p -= 1.0f;
        }
    }
    {
        float p = mutationRates["node"];
        while (p > 0.0f) {
            if (rngf() < p) {
                nodeMutate();
            }
            p -= 1.0f;
        }
    }
    {
        float p = mutationRates["enable"];
        while (p > 0.0f) {
            if (rngf() < p) {
                enableDisableMutate(true);
            }
            p -= 1.0f;
        }
    }
    {
        float p = mutationRates["disable"];
        while (p > 0.0f) {
            if (rngf() < p) {
                enableDisableMutate(false);
            }
            p -= 1.0f;
        }
    }
}