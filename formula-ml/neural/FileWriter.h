#ifndef NEURAL_FILEWRITER
#define NEURAL_FILEWRITER

#include <vector>
#include <map>
#include <string>

#include "neat/Pool.h"
#include "neat/Species.h"
#include "neat/Gene.h"

using namespace std;

namespace neural {

	class FileWriter {
	public:
		FileWriter(std::string root);
		~FileWriter();

        static void clearFile(string filePath);
        static void stringToFile(string filePath, string content);
		void poolToFile(neat::Pool pool, int generation);
		neat::Pool * poolFromFile(std::string path);
		void poolToSingleFile(neat::Pool pool, int generation);
		neat::Pool * poolFromSingleFile(std::string path);
		int extract_generation(string path);
		void genomeToFile(neat::Genome genome, std::string path);


	private:
		std::string root;

		
		neat::Genome * genomeFromFile(std::string path);
		std::string genomePath(std::string root, int species, int genome);

	};

}

#endif