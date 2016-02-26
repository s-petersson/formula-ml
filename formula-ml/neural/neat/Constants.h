#pragma once
#ifndef NEURAL_NEAT_CONSTANTS
#define NEURAL_NEAT_CONSTANTS

namespace neat {
    /** Constants */
    const int Inputs = 2; // 2 + 1 bias
    const int Outputs = 1;

    const int Population = 100;
    const float DeltaDisjoint = 2.0f;
    const float DeltaWeights = 0.4f;
    const float DeltaThreshold = 1.0f;

    const int StaleSpecies = 15;

    const float MutateConnectionsChance = 0.25f;
    const float PerturbChance = 0.90f;
    const float CrossoverChance = 0.75f;
    const float LinkMutationChance = 0.5f;
    const float NodeMutationChance = 0.50f;
    const float BiasMutationChance = 0.40f;
    const float StepSize = 0.1f;
    const float DisableMutationChance = 0.4f;

    const float EnableMutationChance = 0.2f;

    const int MaxNodes = 1000000;

}

#endif