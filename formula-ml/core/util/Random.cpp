#include "Random.h"
#include <random>

using namespace std;

default_random_engine generator;
uniform_real_distribution<float> real_distribution;

float rngf() {
    return real_distribution(generator);
}


float rngf(float min, float max) {
    return min + rngf() * (max - min);
}


int rngi(int count) {
    uniform_int_distribution<int> disc_distribution(0, count-1);
    return disc_distribution(generator);
}

int rngi(int min, int max) {
    uniform_int_distribution<int> disc_distribution(min, max);
    return disc_distribution(generator);
}