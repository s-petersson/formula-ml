#include "Filewriter.h"
#include <fstream>
#include <iostream>


using namespace std;
using namespace neural;
using namespace neat;

FileWriter::FileWriter() {

}
void FileWriter::genomeToFile(Genome genome, string path) {
	ofstream file;
	file.open(path);


	file << genome.fitness << endl;
	file << genome.adjustedFitness << endl;
	file << genome.maxneuron << endl;
	file << genome.globalRank << endl;
	file << "out\tin\tweight\tenabled\tinnovation\tcreated" << endl;

	for (int i = 0; i < genome.genes.size(); i++) {
		
		file << genome.genes.at(i).out << "\t";
		file << genome.genes.at(i).into << "\t";
		file << genome.genes.at(i).weight << "\t";
		file << genome.genes.at(i).enabled << "\t";
		file << genome.genes.at(i).innovation << "\t";
		file << genome.genes.at(i).created << endl;
	}
}
