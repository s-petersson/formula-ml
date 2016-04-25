#include "ResultWriter.h"



using namespace std;
using namespace neural;
using namespace neat;

string headers = "Generation,Best fitness,Best distance,Best time,Avg fitness,Worst fitness,Species";


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
	double best_fitness, best_distance, best_time, sum_fitness, worst_fitness;
	int species;

	file << generation << endl;
	//file << pool.maxFitness <<","<<pool.
}