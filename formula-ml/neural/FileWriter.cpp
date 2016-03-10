#include "Filewriter.h"
#include <fstream>
#include <sstream>

#include <windows.h>


using namespace std;
using namespace neural;
using namespace neat;

FileWriter::FileWriter() {

}

FileWriter::~FileWriter() {

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

//Path with generation given
void FileWriter::poolToFile(Pool pool, string path) {
	
	bool created = CreateDirectory(path.c_str(), NULL);

	for (int s = 0; s < pool.species.size(); s++) {
		ostringstream speciesPath;
		speciesPath << path <<"\\Species_" << s;
		bool secondCreated = CreateDirectory(speciesPath.str().c_str(), NULL);
		for (int g = 0; g < pool.species.at(s).genomes.size(); g++) {
			ostringstream genomePath;
			genomePath << speciesPath.str() << "\\Genome_" << g << "_" << pool.species.at(s).genomes.at(g).fitness;
			genomeToFile(pool.species.at(s).genomes.at(g), genomePath.str()+".txt");
		}

	}
}
