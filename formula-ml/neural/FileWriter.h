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
		FileWriter();
		~FileWriter();

		void genomeToFile(neat::Genome genomde, std::string path);
		void poolToFile(neat::Pool pool, std::string path);
		neat::Pool * poolFromFile(std::string path);


	private:

		neat::Genome * genomeFromFile(std::string path);
		std::string genomePath(std::string root, int species, int genome);

	};

}

#endif