#ifndef NEURAL_NEAT_GENE
#define NEURAL_NEAT_GENE

namespace neat {

    class Gene {
    public:
        
        Gene();
        Gene(int _into, int _out, int _inno);
        Gene(const Gene & gene);
        ~Gene();

        const int into;
        const int out;
        const int innovation;
        float weight;
        bool enabled = true;
        const bool created = false;

        Gene operator=(const Gene& other);
    };
}

#endif