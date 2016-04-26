#ifndef NEURAL_RESULTWRITER
#define NEURAL_RESULTWRITER

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <exception>  

#include "neat/Pool.h"
#include "neat/Species.h"
#include "neat/Gene.h"



namespace neural {

	class ResultWriter {
	public:
		ResultWriter(std::string root);
		~ResultWriter();

		void addNewGeneration( int generation, neat::Pool pool );


	private:
		std::string file_path;
		std::ofstream file;
	};

}

#endif