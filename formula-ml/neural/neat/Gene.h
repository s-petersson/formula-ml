#ifndef NEURAL_NEAT_GENE
#define NEURAL_NEAT_GENE

namespace neat {

    class Gene {
    public:
        int into;
        int out;
        float weight;
        bool enabled = true;
        int innovation = 0;
        bool created = false;

        Gene();
        Gene(const Gene & gene);
        ~Gene();
    };


}

#endif