#include "Gene.h"

using namespace neat;


Gene::Gene() : 
    into(-1), 
    out(-1), 
    innovation(0), 
    created(false) 
{
    enabled = false;
}

Gene::Gene(int _into, int _out, int _inno) :
    into(_into),
    out(_out),
    innovation(_inno),
    created(true)
{
    enabled = true;
}

Gene::Gene(const Gene & gene) :
    into(gene.into),
    out(gene.out),
    innovation(gene.innovation),
    created(gene.created)
{
    weight = gene.weight;
    enabled = gene.enabled;
}

Gene::~Gene() {}


Gene Gene::operator=(const Gene& other) {
    return Gene(other);
}