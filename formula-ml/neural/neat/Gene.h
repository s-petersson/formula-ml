#ifndef NEURAL_NEAT_GENE
#define NEURAL_NEAT_GENE

namespace neat {

    class Gene {
    public:
        
        Gene();
        Gene(int _into, int _out, int _inno);
        Gene(const Gene & gene);
        ~Gene();

        int into;
        int out;
        int innovation;
        float weight;
        bool enabled = true;
        bool created = false;
    };
}

#endif