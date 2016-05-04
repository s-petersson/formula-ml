#include "FileWriter.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <exception>  


using namespace std;
using namespace neural;
using namespace neat;

FileWriter::FileWriter(std::string input) {
	system(("mkdir \"" + input + "\"").c_str());
	root = input;
}

FileWriter::~FileWriter() {

}

/**
 * Takes a path to a file and appends the content that was supplied to the file.
 * This method requires that the file that was supplied already exists.
 * It will not create the file if it doesn't exist.
 */
void FileWriter::stringToFile(string filePath, string content) {
    ofstream file;
    file.open(filePath);

    file << content << endl;
    file.close();
}

void FileWriter::genomeToFile(Genome genome, std::string path) {
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
void FileWriter::poolToFile(Pool pool, int generation) {
	Genome best = pool.species.at(0).genomes.at(0);
	ostringstream generationPath;
	generationPath << root.c_str() << "Generation_" << generation;
	//ostringstream rootCmd;
	//rootCmd << "mkdir \"" << ;
	system(("mkdir \"" + generationPath.str() + "\"").c_str());

	for (int s = 0; s < pool.species.size(); s++) {
		ostringstream speciesPath, speciesPathCmd;
		speciesPath << generationPath.str() << "/Species_" << s;
		speciesPathCmd << "mkdir \"" << speciesPath.str() << "\"";
		system(speciesPathCmd.str().c_str());
		for (int g = 0; g < pool.species.at(s).genomes.size(); g++) {
			if (pool.species.at(s).genomes.at(g).fitness > best.fitness) {
				best = pool.species.at(s).genomes.at(g);
			}
			ostringstream genomePath;
			genomePath << speciesPath.str() << "/Genome_" << g;
			genomeToFile(pool.species.at(s).genomes.at(g), genomePath.str()+".txt");
		}
	}

	genomeToFile(best, generationPath.str() + "/best.txt");

	
}
/*
* Root path for a single pool/generation
*/
Pool * FileWriter::poolFromFile(string path) {
	Pool * pool = new Pool();
	int s = 0;
	int g = 0;
	
	
	ifstream file(genomePath(path, s , g));
	if (!file.is_open()) {
		throw runtime_error("Incorrect path");
	}
	file.close();

	
	//double means end of input
	bool lastFail = false;

	//TODO replace while-true
	while (true) {
		Genome * nextGenome = genomeFromFile(genomePath(path, s, g));
		if (nextGenome == nullptr) {
			if (lastFail) {
				return pool;
			}
			lastFail = true;
			s++;
			g = 0;
		}
		else {
			lastFail = false;
			pool->addToSpecies(*nextGenome);
			cout << "Species " << s << " Genome " << g << endl;
			g++;
			if (nextGenome->fitness > pool->maxFitness) {
				pool->maxFitness = nextGenome->fitness;
				cout << "new max fittnes " << pool->maxFitness;
			}
		}
	}
		
}

string FileWriter::genomePath(string root, int species, int genome) {
	ostringstream file_path;
	file_path << root << "/Species_" << species << "/Genome_" << genome << ".txt";
	return file_path.str();
}

/*
* .txt of a genome file
*/
Genome * FileWriter::genomeFromFile(string path) {
	ifstream file(path);
	if (!file.is_open()) {
		return nullptr;
	}

	Genome * genome = new Genome();
	string line;
	string::size_type st;

	//Data
	getline(file, line);
	float fitness = stof(line, &st);
	getline(file, line);
	float adjustedFitness = stoi(line, &st);
	getline(file, line);
	int maxNeuron = stoi(line, &st);
	getline(file, line);
	int globalRank = stoi(line, &st);
	
	//One format line
	getline(file, line);
	int out, into;
	float weight;
	bool enabled;
	int innovation;
	bool created;
	std::vector<Gene> genes;


	while (getline(file, line)) {
		istringstream in(line);
		in >> out >> into >> weight >> enabled >> innovation >> created;
		Gene gene;
		gene.out = out;
		gene.into = into;
		gene.weight = weight;
		gene.enabled = enabled;
		gene.innovation = innovation;
		gene.created = created;

		genes.push_back(gene);
	}

	genome->adjustedFitness = adjustedFitness;
	genome->fitness = fitness;
	genome->maxneuron = maxNeuron;
	genome->globalRank = globalRank;
	genome->genes = genes;

	return genome;
}
