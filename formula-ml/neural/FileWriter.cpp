#include "Filewriter.h"
#include <fstream>
#include <sstream>
#include <direct.h>
#include <windows.h>
#include <exception>  


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
	
	int created = _mkdir(path.c_str());

	for (int s = 0; s < pool.species.size(); s++) {
		ostringstream speciesPath;
		speciesPath << path <<"\\Species_" << s;
		int secondCreated = _mkdir(speciesPath.str().c_str());
		for (int g = 0; g < pool.species.at(s).genomes.size(); g++) {
			ostringstream genomePath;
			genomePath << speciesPath.str() << "\\Genome_" << g;
			genomeToFile(pool.species.at(s).genomes.at(g), genomePath.str()+".txt");
		}

	}
}
/*
* Root path for a single pool/generation
*/
Pool FileWriter::poolFromFile(string path) {
	Pool * pool = new Pool();
	int s = 0;
	int g = 0;
	ostringstream filePath;
	filePath << path << "\\Species_" << s << "\\Genome_" << g << ".txt";

	ifstream file;
	if (!file.good()) {
		throw runtime_error("Incorrect path");
	}

	while (true) {
		
	}
		
}

/*
* .txt of a genome file
*/
Genome FileWriter::genomefromFile(string path) {

}
