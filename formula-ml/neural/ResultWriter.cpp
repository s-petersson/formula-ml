#include "ResultWriter.h"



using namespace std;
using namespace neural;
using namespace neat;

string headers = "Generation,Best fitness,Avg fitness,Worst fitness,Species,Pool size";


ResultWriter::ResultWriter(std::string input) {
	file_path = input;
	file.open(file_path);
	file << headers;
	for (int i = 0; i < Population; i++) {
		file << ",Genome " << i;
	}
	file << endl;
}

ResultWriter::~ResultWriter() {

}

void ResultWriter::addNewGeneration(int generation, neat::Pool pool) {
	double best_fitness = 0; 
	double sum_fitness = 0;
	double worst_fitness = 0;
	int species=0;
	ostringstream genome_values;
	for (int s = 0; s < pool.species.size(); s++) {
		species++;
		Species species = pool.species.at(s);
		for (int g = 0; g < species.genomes.size(); g++) {
			Genome genome = species.genomes.at(g);
			if (genome.fitness > best_fitness) {
				best_fitness = genome.fitness;
			}
			genome_values << "," << genome.fitness;
			sum_fitness += genome.fitness;
			if (worst_fitness > genome.fitness || worst_fitness == 0) {
				worst_fitness = genome.fitness;
			}
		}
	}

	file << generation << "," << best_fitness << "," << sum_fitness / Population << "," << worst_fitness << "," << species << "," << Population;
	file <<	genome_values.str() << endl;
}