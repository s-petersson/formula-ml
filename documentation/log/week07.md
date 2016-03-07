# Week 7, February 29 - March 6


## Gabriel

### Time in fitness function


### Track curve
Implemented a indata preprocessor that takes the length and angles to the next number of checkpoints.

### Heykin
One often want the network to be able to handle invariants of a class. The structure of the network may impose the same result different invariants. Example from image processing. The same recognition pattern structure is repeated, translated and rotated. [Comment, is this not what convolutional neural net is about?]

Two types of stored knowledge, page 57: Declarative: knowledge defined with symbols. Procedural: The "knowledge" is embedded in the executable code.

Dynamic programming is often used in conjecture with reinforcement learning in the realm of Markov Decision Problems. Q-learning also relate MDP.
  
### Evolving Neural Networks through augmenting topologies
NEAT uses a direct mapping between genome and phenotype. +Simplicity, -Size. Indirect mappings may be more compact, but may bias the topology in unpredicted ways. 
NEAT is able to determine gene homology, that two genes are the same for two different genomes. This enable crossover breeding without loosing features due to invariants.
NEAT separates different types of genomes in different species. Individuals only compete internally in a species. This make the possibility for invention survive a temporary decrease in fitness. [Comment: How much is it able to evolve without increasing fitness? May it need specific assistance in any case?]

NEAT starts with a minimalistic structure. It optimises and increase in complexity as the solution progress. This lead to a simpler solution that is faster to find than many other approaches. It is therefore not as necessary to penalise network size in the fitness function.


## Daniel

### Halftime presentation

We managed to do a decent halftime presentation in my oppinion. We got the question "How do you program this?" after the presentation, and that is about it. This is quite interesting as this tells us that the audience might not have understood everything as clear as we thought they would. 


We went over a the racing domain as well as how reinforcement learning works, but I think that the audience thought everything to be to abstract to get a good understanding of. It is a complex subject that we do adress, but the report need to address this problem that we realised.