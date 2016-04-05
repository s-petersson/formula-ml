#ifndef NEURAL_FILEWRITER
#define NEURAL_FILEWRITER

#include <vector>
#include <map>
#include <string>

#include "neat/Pool.h"
#include "neat/Species.h"
#include "neat/Gene.h"



namespace neural {

	class FileWriter {
	public:
		FileWriter(std::string root);
		~FileWriter();

		void genomeToFile(neat::Genome genome, std::string path);
		void poolToFile(neat::Pool pool, int generation);
		neat::Pool * poolFromFile(std::string path);


	private:
		std::string root;

		neat::Genome * genomeFromFile(std::string path);
		std::string genomePath(std::string root, int species, int genome);

	};

}

#endif