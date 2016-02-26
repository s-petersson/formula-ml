#include "NeatXOR.h"
#include <iostream>
#include <cmath>
#include <vector>
#include <set>
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
const int Inputs = 2; // 2 + 1 bias
const int Outputs = 1;

const int Population = 100;
const float DeltaDisjoint = 2.0f;
const float DeltaWeights = 0.4f;
const float DeltaThreshold = 1.0f;

const int StaleSpecies = 15;

const float MutateConnectionsChance = 0.25f;
const float PerturbChance = 0.90f;
const float CrossoverChance = 0.75f;
const float LinkMutationChance = 0.5f;
const float NodeMutationChance = 0.50f;
const float BiasMutationChance = 0.40f;
const float StepSize = 0.1f;
const float DisableMutationChance = 0.4f;

const float EnableMutationChance = 0.2f;

const int MaxNodes = 1000000;

/** Sigmoid function */
float sigmoid(float x) {
    return 2.0f / (1.0f + glm::exp(-4.9f * x)) - 1.0f;
    //return 1.0f / (1.0f + glm::exp(-2.0f*x));
}

/** Data types */

/** A gene is a connection in the network. */
struct Gene {
    int into;
    int out;
    float weight;
    bool enabled = true;
    int innovation = 0;
    bool created = false;
};

Gene createGene() {
    Gene gene;
    gene.into = -1;
    gene.out = -1;
    gene.weight = 0.0f;
    gene.enabled = true;
    gene.innovation = 0;
    gene.created = true;
    return gene;
}

/** A neuron has a value and a set of incoming edgees (Genes)*/
struct Neuron {
    vector<Gene> incoming;
    float value = 0.0f;
    bool created = false;
};

Neuron createNeuron() {
    Neuron neuron;
    neuron.value = 0.0f;
    neuron.created = true;
    return neuron;
}

/** A network is just a collection of Neurons. */
struct Network {
    //Neuron* neurons;
    //int neuron_count;
    map<int, Neuron> neurons;
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
    int staleness;
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

Pool pool;

int newInnovation() {
    pool.innovation++;
    return pool.innovation;
}

/** Generates a genomes neural net. */
void generateNetwork(Genome& genome) {
    Network network;
    //network.neuron_count = genome.genes.size() + Inputs + Outputs;
    //network.neurons = new Neuron[network.neuron_count];
   
    for (int i = 1; i <= Inputs; i++) {
        network.neurons[i] = createNeuron();
    }
    for (int i = 1; i <= Outputs; i++) {
        network.neurons[MaxNodes + i] = createNeuron();
    }

    sort(genome.genes.begin(), genome.genes.end(), [](Gene a, Gene b) {return a.out < b.out; });

    for (int i = 0; i < genome.genes.size(); i++) {
        Gene g = genome.genes[i];
        if (g.enabled) {
            /*
            if (network.neurons[g.out].incoming.size() == 0) { // 0 check will probably be iffy, maybe use pointers or flags instead? Or maybe set memory values.
                network.neurons[g.out] = createNeuron();
            }*/
            if (!network.neurons[g.out].created) {
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
    for (auto && n : network.neurons) {
        n.second.value = 0.0f;
    }
    
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
        network.neurons[i+1].value = inputs[i];
    }
    // Calculate all the nodes, the sorting of the genes should!!! make this work.
    for (auto && node : network.neurons) {
        for (auto && g : node.second.incoming) {
            node.second.value += g.weight * network.neurons[g.into].value;
        }
        // The "original" uses a > 0 check here before using the sigmoid function.
        node.second.value = sigmoid(node.second.value);
    }

    for (int i = 0; i < Outputs; i++) {
        //outputs[i] = network.neurons[network.neuron_count - Outputs + i].value; // Might need to change the position of the outputs in the list.
        outputs[i] = network.neurons[MaxNodes + i + 1].value;
        //outputs[i] = 7.0f;
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
    set<int> neurons;

    if (input) {
        for (int i = 1; i <= Inputs; i++) {
            neurons.insert(i);
        }
    }
    for (int i = 1; i <= Outputs; i++) {
        neurons.insert(MaxNodes + i);
    }

    for (auto && gene : genes) {
        if (gene.into > Inputs) neurons.insert(gene.into);
        neurons.insert(gene.out);
    }
    int i = rngi(neurons.size());
    for (auto && v : neurons) {
        if (i == 0) return v;
        i--;
    }
    return *neurons.end(); // should never occur
    /*
    map<int, bool> neurons;
    if (input) {
        for (int i = 1; i <= Inputs; i++) {
            neurons[i] = true;
        }
    }

    for (int i = 1; i <= Outputs; i++) {
        //neurons[genes.size() + Inputs + i] = true;
        neurons[MaxNodes + i] = true;
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

    return 0;*/
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
    int neuron1 = randomNeuron(genome.genes, true);
    int neuron2 = randomNeuron(genome.genes, false);
    
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
        newLink.into = Inputs; // Changed the Bias node index to index 0.
    }
    
    // Dont add duplicates
    if (containsLink(genome.genes, newLink)) return;

    newLink.innovation = newInnovation();
    newLink.weight = rngf() * 4.0f - 2.0f;

    //cout << "NEW LINK " << newLink.into << " -> " << newLink.out << endl;
    genome.genes.push_back(newLink);
}

/** Splits an edge into two. */
void nodeMutate(Genome& genome) {
    if (genome.genes.size() == 0) return;
    genome.maxneuron++;
    
    Gene* gene = &genome.genes[rngi(genome.genes.size())];
    if (!gene->enabled) return;
    // disable the old link
    gene->enabled = false;

    // Add the two new links. 
    Gene gene1 = *gene;
    gene1.out = genome.maxneuron;
    gene1.weight = 1.0f;
    gene1.innovation = newInnovation();
    gene1.enabled = true;
    genome.genes.push_back(gene1);

    Gene gene2 = *gene;
    gene2.into = genome.maxneuron;
    gene2.innovation = newInnovation();
    gene2.enabled = true;
    genome.genes.push_back(gene2);
    
}

void enableDisableMutate(Genome& genome, bool enable) {
    vector<Gene*> candidates;
    for (int i = 0; i < genome.genes.size(); i++) {
        Gene * gene = &genome.genes[i];
        if (gene->enabled != enable) candidates.push_back(gene);
    }
    if (candidates.size() == 0) return;

    Gene* gene = candidates[rngi(candidates.size())];
    gene->enabled = !gene->enabled;
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
    return (float) disjointGenes / n;
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

bool sameSpecies(const Genome& genome1, const Genome& genome2) {
    float dd = DeltaDisjoint*disjoint(genome1.genes, genome2.genes);
    float dw = DeltaWeights*weights(genome1.genes, genome2.genes);
    return (dd + dw) < DeltaThreshold;
}

// Assigns each genome its global rank. 
void rankGlobally() {
    vector<Genome*> global;
    for (auto && s : pool.species) {
        for (int i = 0; i < s.genomes.size(); i++) {
            global.push_back(&s.genomes[i]);
        }
    }
    sort(global.begin(), global.end(), [](Genome* g1, Genome* g2) {return g1->fitness < g2->fitness;});

    for (int i = 0; i < global.size(); i++) {
        global[i]->globalRank = i;
    }
}

void calculateAverageFitness(Species& species) {
    int total = 0;

    for (const auto & genome : species.genomes) {
        total += genome.globalRank;
    }
    species.averageFitness = total / species.genomes.size();
}

// Why not just averagefitness?
float totalAverageFitness() {
    float total = 0.0f;
    for (const auto & s : pool.species) {
        total += s.averageFitness;
    }
    return total;
}

void cullSpecies(bool cutToOne) {
    for (auto && s : pool.species) {
        sort(s.genomes.begin(), s.genomes.end(), [](Genome a, Genome b) {return a.fitness > b.fitness; });
        int remove = s.genomes.size() / 2; // Cut half the population. 
        if (cutToOne) remove = s.genomes.size() - 1;
        s.genomes.erase(s.genomes.end() - remove, s.genomes.end());
    }
}

Genome breedChild(Species& species) {
    Genome child;
    if (rngf() < CrossoverChance) {
        Genome g1 = species.genomes[rngi(species.genomes.size())];
        Genome g2 = species.genomes[rngi(species.genomes.size())];
        child = crossover(g1, g2);
    }
    else {
        child = species.genomes[rngi(species.genomes.size())];
    }
    mutate(child);
    return child;
}

void removeStaleSpecies() {
    vector<Species> survived;
    
    for (auto && species : pool.species) {
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

        if (species.staleness < StaleSpecies || species.topFitness >= pool.maxFitness) {
            survived.push_back(species);
        }
    }
    pool.species = survived;
}

void removeWeakSpecies() {
    vector<Species> survived;

    float sum = totalAverageFitness();
    for (int i = 0; i < pool.species.size(); i++) {
        Species species = pool.species[i];
        // Actually quite inefficient to actutally sort to find the max, better to just search.
        int breed = (int)glm::floor(species.averageFitness / sum * Population); // This is just weird, test whether this actually works.
        if (breed >= 1) {
            survived.push_back(species);
        }
    }
    pool.species = survived;
}

void addToSpecies(Genome child) {
    bool foundSpecies = false;
    for (auto && s : pool.species) {
        if (sameSpecies(child, s.genomes[0])) {
            //cout << "Found species!" << endl;
            s.genomes.push_back(child);
            foundSpecies = true;
            break;
        }
    }

    if (!foundSpecies) {
        Species childSpecies = createSpecies();
        childSpecies.genomes.push_back(child);
        pool.species.push_back(childSpecies);
    }
}

Genome basicGenome() {
    Genome genome = createGenome();
    genome.maxneuron = Inputs;
    mutate(genome);
    return genome;
}

void newGeneration() {
    cullSpecies(false);
    rankGlobally();
    removeStaleSpecies();
    rankGlobally();
    for (auto && s : pool.species) {
        calculateAverageFitness(s);
    }
    removeWeakSpecies();
    
    float sum = totalAverageFitness();
    vector<Genome> children;
    for (auto && s : pool.species) {
        int breed = (int)glm::floor(s.averageFitness / sum * Population) - 1; // What is this for?
        for (int i = 0; i < breed; i++) {
            children.push_back(breedChild(s));
        }
    }
    cullSpecies(true);
    while (children.size() + pool.species.size() < Population) {
        Species s = pool.species[rngi(pool.species.size())];
        children.push_back(breedChild(s));
    }
    for (auto && i : children) {
        addToSpecies(i);
    }
    pool.generation++;

    // Write state to file to backup!!!!!  
}

void initializePool() {
    pool = createPool();
    for (int i = 0; i < Population; i++) {
        addToSpecies(basicGenome());
    }
    cout << "Number of species" << pool.species.size() << endl;
}

void evaluateFitness(Genome& genome) {

    float fitness = 0.0f;
    float * inputs = new float[2];
    float * outputs = new float[1];
    inputs[1] = 1.0f;
    generateNetwork(genome);

    for (int i = 0; i < 100; i++) {
        float in = rngf(0.0f, 2.0f) - 1.0f;
        inputs[0] = in;
        evaluateNetwork(genome.network, inputs, 2, outputs, 1);
        if (in > 0.0f) {
            if (outputs[0] > 0.5f) fitness -= 1.0f;
            if (outputs[0] < -0.5f) fitness += 1.0f;
        }
        if (in < 0.0f) {
            if (outputs[0] > 0.5f) fitness += 1.0f;
            if (outputs[0] < -0.5f) fitness -= 1.0f;
        }
    }
    genome.fitness = fitness;
    if (fitness > pool.maxFitness) {
        pool.maxFitness = fitness;
        cout << "New maximum fitness: " << fitness << " Nodes: " << genome.network.neurons.size() << endl;
    }

    /*
    float fitness = 0.0f;
    float * inputs = new float[3];
    float * outputs = new float[1];
    generateNetwork(genome);
    float o1, o2, o3, o4;
    inputs[2] = 1.0f;
    float distance = 0.0f;
    
    inputs[0] = 0.0f;
    inputs[1] = 0.0f;
    outputs[0] = 0.0f;
    evaluateNetwork(genome.network, inputs, 3, outputs, 1);
    o1 = outputs[0];
    distance += o1;
    //cout << outputs[0];

    inputs[0] = 0.0f;
    inputs[1] = 1.0f;
    outputs[0] = 0.0f;
    evaluateNetwork(genome.network, inputs, 3, outputs, 1);
    o2 = outputs[0];
    distance += glm::abs(1.0f - o2);
    //cout << outputs[0];

    inputs[0] = 1.0f;
    inputs[1] = 0.0f;
    outputs[0] = 0.0f;
    evaluateNetwork(genome.network, inputs, 3, outputs, 1);
    o3 = outputs[0];
    distance += glm::abs(1.0f - o3);
    //cout << outputs[0];

    inputs[0] = 1.0f;
    inputs[1] = 1.0f;
    outputs[0] = 0.0f;
    evaluateNetwork(genome.network, inputs, 3, outputs, 1);
    o4 = outputs[0];
    distance += o4;
    //cout << outputs[0] << endl;

    fitness = (4.0f - distance);
    fitness *= fitness;
    //cout << "Genome #" << genome.genes.size() << "Fitness: " << fitness << endl;
    genome.fitness = fitness;
    if (fitness > pool.maxFitness) {
        pool.maxFitness = fitness;
        cout << "New maximum fitness: " << fitness << " Nodes: "<< genome.network.neurons.size() << endl;
        cout << "Outputs : " << o1 << " " << o2 << " " << o3 << " " << o4 << endl;
        for (auto && e : genome.genes) {
            cout << e.into << " -> " << e.out << " : " << e.weight << endl;
        }
    }
    */
}


void neatxor::train() {
   
    initializePool();
    while (true) {
        for (auto && species : pool.species){
            for (auto && genome : species.genomes) {
                evaluateFitness(genome);
            }
        }
        newGeneration();
        cout << "New generation. Number of species: " << pool.species.size() << endl;
    }
}
