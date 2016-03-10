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

		void poolToFile(neat::Pool pool, std::string path);
		//neat::Pool poolFromFile(std::string path);


	private:
		std::string path;

		void genomeToFile(neat::Genome genomde, std::string path);
		//neat::Genome genomefromFile(std::string path);

	};

}

#endif