#include "Gene.h"

using namespace neat;


Gene::Gene() {
    into        = -1;
    out         = -1;
    innovation  = 0;
    created     = true;
}

Gene::Gene(const Gene & gene) {
    into        = gene.into;
    out         = gene.out;
    weight      = gene.weight;
    created     = gene.created;
    innovation  = gene.innovation;
}

Gene::~Gene() {}