#ifndef NEURAL_NEAT_CONSTANTS
#define NEURAL_NEAT_CONSTANTS
#include <functional>
#include <glm/glm.hpp>

namespace neat {
    class Config {
    public:

        static void set_config(int inputs, int outputs);
        /** Constants */
        static int Inputs; //= 3 + 10; // 2 + 1 bias
        static int Outputs;// = 1;
        static bool InitialStructure;

        static std::function<float(float x)> sigmoid;
    };

    const float DeltaDisjoint = 2.0f;
    const float DeltaWeights = 0.4f;
    const float DeltaThreshold = 1.0f;

    const int Population = 500;
    const float MutateConnectionsChance = 0.70f;
    const float PerturbChance = 0.90f;
    const float CrossoverChance = 0.75f;
    const float LinkMutationChance = 0.50f;
    const float NodeMutationChance = 0.40f;
    const float BiasMutationChance = 0.40f;
    const float StepSize = 0.1f;
    const float DisableMutationChance = 0.4f;
    const float EnableMutationChance = 0.2f;
    const int StaleSpecies = 20;

    const int MaxNodes = 1000000;
}

#endif