#ifndef NEURAL_FIXEDNETWORKXOR
#define NEURAL_FIXEDNETWORKXOR
#include "FixedNetwork.h"

namespace neural{

    class FixedNetworkXOR {
    public:
        FixedNetworkXOR();
        ~FixedNetworkXOR();

        void train();

    private:
        FixedNetwork * network;
    };

} // namespace neural

#endif