#include <iostream>
#include <cmath>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <glm/glm.hpp>
#include <core/util/Random.h>
using namespace std;

/**
* A translation of the MarI/O lua script to C++. The code is written in the style of C to ease the translation from lua.
*/

/** Constants */
const int Inputs = 3; // Number of inputs + one bias. 
const int Outputs = 1;

const int Population = 300;
const float DeltaDisjoint = 2.0;
const float DeltaWeights = 0.4;
const float DeltaThreshold = 1.0;

const int StaleSpecies = 15;

const float MutateConnectionsChance = 0.25;
const float PerturbChance = 0.90;
const float CrossoverChance = 0.75;
const float LinkMutationChance = 2.0;
const float NodeMutationChance = 0.50;
const float BiasMutationChance = 0.40;
const float StepSize = 0.1;
const float DisableMutationChance = 0.4;
const float EnableMutationChance = 0.2;

const int MaxNodes = 1000000;

/** Sigmoid function */
float sigmoid(float x) {
    return 2.0f / (1.0f + glm::exp(-4.9f * x)) - 1.0f;
}

/** Data types */

/** A gene is a connection in the network. */
struct Gene {
    int into;
    int out;
    float weight;
    bool enabled = true;
    int innovation = 0;
};

Gene createGene() {
    Gene gene;
    gene.into = -1;
    gene.out = -1;
    gene.weight = 0.0f;
    gene.enabled = true;
    gene.innovation = 0;
    return gene;
}

/** A neuron has a value and a set of incoming edgees (Genes)*/
struct Neuron {
    vector<Gene> incoming;
    float value = 0.0f;
};

Neuron createNeuron() {
    Neuron neuron;
    neuron.value = 0.0f;
    return neuron;
}

/** A network is just a collection of Neurons. */
struct Network {
    Neuron* neurons;
    int neuron_count;
};

/** A genome or set of genes is sort of a blueprint for a network. */
struct Genome {
    vector<Gene> genes;
    map<string, float> mutationRates;
    float fitness = 0;
    float adjustedFitness = 0;

    Network network;
    int maxneuron = 0;
    int globalRank = 0;
};

Genome createGenome() {
    Genome genome;
    genome.fitness = 0.0f;
    genome.adjustedFitness = 0.0f;
    genome.mutationRates["connections"] = MutateConnectionsChance;
    genome.mutationRates["link"] = LinkMutationChance;
    genome.mutationRates["bias"] = BiasMutationChance;
    genome.mutationRates["node"] = NodeMutationChance;
    genome.mutationRates["enable"] = EnableMutationChance;
    genome.mutationRates["disable"] = DisableMutationChance;
    genome.mutationRates["step"] = StepSize;
    return genome;
}

/** A species is a collection of related genomes. */
struct Species {
    vector<Genome> genomes;
    float topFitness;
    float staleness;
    float averageFitness;
};

Species createSpecies() {
    Species species;

    species.topFitness = 0;
    species.staleness = 0;
    species.averageFitness = 0;

    return species;
}

struct Pool {
    vector<Species> species;
    int generation;
    int innovation;
    int currentSpecies;
    int currentGenome;
    int currentFrame;
    float maxFitness;

    
};


Pool createPool() {
    Pool pool;

    pool.generation = 0;
    pool.innovation = Outputs;
    pool.currentSpecies = 1;
    pool.currentGenome = 1;
    pool.currentFrame = 0;
    pool.maxFitness = 0;

    return pool;
}


Pool pool = createPool();

int newInnovation() {
    pool.innovation++;
    return pool.innovation;
}

/** Generates a genomes neural net. */
void generateNetwork(Genome& genome) {
    Network network;
    network.neuron_count = genome.genes.size() + Inputs + Outputs;
    network.neurons = new Neuron[network.neuron_count];
   
    for (int i = 0; i < Inputs; i++) {
        network.neurons[i] = createNeuron();
    }
    for (int i = 0; i < Outputs; i++) {
        network.neurons[network.neuron_count - Outputs + i] = createNeuron();
    }

    sort(genome.genes.begin(), genome.genes.end(), [](Gene a, Gene b) {return a.out < b.out; });

    for (int i = 0; i < genome.genes.size(); i++) {
        Gene g = genome.genes[i];
        if (g.enabled) {
            if (network.neurons[g.out].incoming.size() == 0) { // 0 check will probably be iffy, maybe use pointers or flags instead? Or maybe set memory values.
                network.neurons[g.out] = createNeuron();
            }

            network.neurons[g.out].incoming.push_back(g);

            if (network.neurons[g.into].incoming.size() == 0) {
                network.neurons[g.into] = createNeuron();
            }
        }
    }
    genome.network = network;
}

/** Fills the array of outputs by computing the network with the provided inputs. */
void evaluateNetwork(Network network, float* inputs, int input_count, float* outputs, int output_count) {
    if (input_count != Inputs) {
        cout << "Incorrect number of inputs." << endl;
        return;
    }

    if (output_count != Outputs) {
        cout << "Incorrect number of outputs." << endl;
        return;
    }

    // Fill the input nodes with values.
    for (int i = 0; i < Inputs; i++) {
        network.neurons[i].value = inputs[i];
    }
    // Calculate all the nodes, the sorting of the genes should!!! make this work.
    for (int i = Inputs; i < network.neuron_count; i++) {
        float sum = 0.0f;
        for (auto && g : network.neurons[i].incoming) {
            sum += g.weight * network.neurons[g.into].value;
        }
        // The "original" uses a > 0 check here before using the sigmoid function.
        network.neurons[i].value = sigmoid(sum);
    }

    for (int i = 0; i < Outputs; i++) {
        outputs[i] = network.neurons[network.neuron_count - Outputs + i].value; // Might need to change the position of the outputs in the list.
    }
}

Genome crossover(Genome g1, Genome g2) {
    if (g2.fitness > g1.fitness) {
        Genome temp = g1;
        g1 = g2;
        g2 = temp;
    }
    Genome child;
    map<int, Gene> innovations2;
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

/** Returns the index of a random neuron within a network. Input neurons will only be returned if input = true.*/
int randomNeuron(vector<Gene> genes, bool input) {
    map<int, bool> neurons;
    if (input) {
        for (int i = 0; i < Inputs; i++) {
            neurons[i] = true;
        }
    }

    for (int i = 0; i < Outputs; i++) {
        neurons[genes.size() + Inputs + i] = true;
    }

    for (auto && g : genes) {
        if (input || g.into > Inputs) {
            neurons[g.into] = true;
        }
        if (input || g.out > Inputs) {
            neurons[g.out] = true;
        }
    }
    int n = rngi(neurons.size());

    // Wtf? Returns the index of one of the neurons. 
    for (auto && i : neurons) {
        n--;
        if (n == 0) {
            return i.first;
        }
    }

    return 0;
}

bool containsLink(vector<Gene> genes , Gene link) {
    for (auto && gene : genes) {
        if (gene.into == link.into && gene.out == link.out) return true;
    }
    return false;
}

/** Mutates the weights of a genome, large chance that each weight will be slightly altered, small chance to reset*/
void pointMutate(Genome& genome) {
    float step = genome.mutationRates["step"];
    for (auto && gene : genome.genes) {
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
void linkMutate(Genome& genome, bool forceBias) { 
    int neuron1 = randomNeuron(genome.genes, false);
    int neuron2 = randomNeuron(genome.genes, true);
    
    Gene newLink = createGene();

    // No links between input nodes.
    if (neuron1 < Inputs && neuron2 < Inputs) return;
    if (neuron2 < Inputs) { // No edges into the inputs.
        int temp = neuron2;
        neuron2 = neuron1;
        neuron1 = temp;
    }
    newLink.into = neuron1;
    newLink.out = neuron2;
    if (forceBias) {
        newLink.into = 0; // Changed the Bias node index to index 0.
    }
    // Dont add duplicates
    if (containsLink(genome.genes, newLink)) return;

    newLink.innovation = newInnovation();
    newLink.weight = rngf() * 4.0f - 2.0f;
    genome.genes.push_back(newLink);
}

/** Splits an edge into two. */
void nodeMutate(Genome& genome) {
    if (genome.genes.size() == 0) return;
    genome.maxneuron++;
    
    Gene gene = genome.genes[rngi(genome.genes.size())];
    if (!gene.enabled) return;
    // disable the old link
    gene.enabled = false;

    // Add the two new links. 
    Gene gene1 = gene;
    gene1.out = genome.maxneuron;
    gene1.weight = 1.0f;
    gene1.innovation = newInnovation();
    gene1.enabled = true;
    genome.genes.push_back(gene1);

    Gene gene2 = gene;
    gene2.into = genome.maxneuron;
    gene2.innovation = newInnovation();
    gene2.enabled = true;
    genome.genes.push_back(gene2);
}

void enableDisableMutate(Genome& genome, bool enable) {
    vector<Gene&> candidates;
    for (auto && i : genome.genes) {
        if (i.enabled != enable) candidates.push_back(i);
    }
    if (candidates.size() == 0) return;

    Gene& gene = candidates[rngi(candidates.size())];
    gene.enabled = !gene.enabled;
}
/** Does all the mutation stuff */
void mutate(Genome& genome) {
    for (auto && i : genome.mutationRates) {
        if (rngi(2) == 1) {
            i.second *= 0.95f;
        }
        else {
            i.second *= 1.05263f; // Magic numbers?
        }
    }

    if (rngf() < genome.mutationRates["connections"]) {
        pointMutate(genome);
    }
    {
        float p = genome.mutationRates["link"];
        while (p > 0.0f) {
            if (rngf() < p) {
                linkMutate(genome, false);
            }
            p -= 1.0f;
        }
    }
    {
        float p = genome.mutationRates["bias"];
        while (p > 0.0f) {
            if (rngf() < p) {
                linkMutate(genome, true);
            }
            p -= 1.0f;
        }
    }
    {
        float p = genome.mutationRates["node"];
        while (p > 0.0f) {
            if (rngf() < p) {
                nodeMutate(genome);
            }
            p -= 1.0f;
        }
    }
    {
        float p = genome.mutationRates["enable"];
        while (p > 0.0f) {
            if (rngf() < p) {
                enableDisableMutate(genome, true);
            }
            p -= 1.0f;
        }
    }
    {
        float p = genome.mutationRates["disable"];
        while (p > 0.0f) {
            if (rngf() < p) {
                enableDisableMutate(genome, false);
            }
            p -= 1.0f;
        }
    }
}
