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
		std::ofstream generation_info;
		std::ofstream sorted_genomes;
		//std::ofstream single_line_genome;


	};

}

#endif