#include "Random.h"
#include <random>

using namespace std;

default_random_engine generator;
uniform_real_distribution<float> distribution;

float rngf() {
    return distribution(generator);
}


float rngf(float min, float max) {
    return min + rngf() * (max - min);
}