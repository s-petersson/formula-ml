#ifndef NEURAL_FIXEDNETWORK
#define NEURAL_FIXEDNETWORK

#include "Neural.h"

namespace neural {

    class FixedNetwork : public Network {
    public:
        FixedNetwork();
        ~FixedNetwork();

        virtual NetworkIO& fire(NetworkIO input);

        virtual int inputSize();
        virtual int outputSize();

    private:
        int input_size;
        int output_size;
        int hidden_layer_count;
        int hidden_layer_size;

    };

} // namespace neural 

#endif