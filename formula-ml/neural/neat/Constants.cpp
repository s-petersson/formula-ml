#include "Constants.h"

using namespace neat;

int Config::Inputs = 0;
int Config::Outputs = 0;

void Config::set_config(int in, int out) {
    Inputs = in;
    Outputs = out;
}