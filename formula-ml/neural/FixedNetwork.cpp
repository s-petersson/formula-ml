#include "FixedNetwork.h"

using namespace neural;

FixedNetwork::FixedNetwork() {

}

FixedNetwork::~FixedNetwork() {

}

NetworkIO& FixedNetwork::fire(NetworkIO input) {
    NetworkIO out;
    return out;
}

int FixedNetwork::inputSize() {
    return 0;
}

int FixedNetwork::outputSize() {
    return 0;
}