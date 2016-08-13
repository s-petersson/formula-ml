#include "Constants.h"

using namespace neat;

int Config::Inputs = 0;
int Config::Outputs = 0;
bool Config::InitialStructure = false;

std::function<float(float x)> Config::sigmoid = [](float x) {
    return 1.0f / (1.0f + glm::exp(-x));
};

void Config::set_config(int in, int out) {
    Inputs = in;
    Outputs = out;
}
