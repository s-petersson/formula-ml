#include "ResultWriter.h"
#include <list>



using namespace std;
using namespace neural;
using namespace neat;

string headers = "Generation,Best fitness,Avg fitness,Worst fitness,Species,Pool size";
string complete = "complete.csv";
string sorted = "sorted.csv";
//string single = "single_line.csv";


ResultWriter::ResultWriter(std::string root) {
	generation_info.open(root + complete);
	generation_info << headers;
	sorted_genomes.open(root + sorted);
	sorted_genomes << "Generation";
	//single_line_genome.open(root + single);
	//single_line_genome << "Generation,Species,Fitness" << endl;

	for (int i = 0; i < Population; i++) {
		generation_info << ",Genome " << i;
		sorted_genomes << ",Genome " << i;
	}

	generation_info << endl;
	sorted_genomes << endl;

}

ResultWriter::~ResultWriter() {

}

void ResultWriter::addNewGeneration(int generation, neat::Pool pool) {
	std::list<float> fitness_list;
	double sum_fitness = 0;
	float max_fitness = 0;
	float min_fitness = 0;
	int species=0;
	ostringstream genome_values;
	for (int s = 0; s < pool.species.size(); s++) {
		species++;
		Species species = pool.species.at(s);
		for (int g = 0; g < species.genomes.size(); g++) {
			Genome genome = species.genomes.at(g);
			
			genome_values << "," << genome.fitness;
			//single_line_genome << generation << "," << s << "," << genome.fitness << endl;
			sum_fitness += genome.fitness;
			fitness_list.push_back(genome.fitness);
		}
	}
	fitness_list.sort();
	max_fitness = fitness_list.back();
	min_fitness = fitness_list.front();
	sorted_genomes << generation;
	for (int i = 0; i < Population; i++) {
		sorted_genomes << "," << fitness_list.front();
		fitness_list.pop_front();
	}
	sorted_genomes << endl;

	generation_info << generation << "," << max_fitness << "," << sum_fitness / Population << "," << min_fitness << "," << species << "," << Population;
	generation_info <<	genome_values.str() << endl;
}