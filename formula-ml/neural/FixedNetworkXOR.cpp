#include "FixedNetworkXOR.h"

using namespace neural;

FixedNetworkXOR::FixedNetworkXOR() {
    network = new FixedNetwork(2,1,1,1);
}

FixedNetworkXOR::~FixedNetworkXOR() {
    if (network) delete network;
}

void FixedNetworkXOR::train() {

}
