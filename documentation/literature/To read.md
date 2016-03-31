# Techniques 

### NEAT

### NEAT with heurustics?

### ESP?
Mentioned as a preceeding algorithm to CoSyNE

### Q-learning
Appears often together with reinforcement learning in the context of Markov decision problem

### Dynamic programming
Appears often together with reinforcement learning in the context of Markov decision problem

### Linear programming
Said to scale badly when the state space become large, for example a continius space (CoSyNE paper)




# Top priority papers
### Deep learning in neural networks: An overview 
(Simon)
Hot paper & Highly cited
Review of a number of different algorithms
cited Evolving Neural Networks through augmenting topologies

### Combining NEAT and PSO for learning tactical human behavior
Probably discusses relevant problems concerning planning and strategy! Discusses an algorithm called PIGEON.
cited Evolving Neural Networks through augmenting topologies

### Accelerated neural evolution through cooperatively coevolved synapses (CoSyNE)
(Gabriel)
published 2008
Compares a neuroevolution method called Cooperative Synapse Neuroevolution (CoSyNE) to other reinforcement algorithms.
Benchmark tests the pole balancing problem.
cited Evolving Neural Networks through augmenting topologies

#### Notes
Introduction: Problem targeted (1.) non-linear and noisy, difficult to engineer a models, (2.) and complex so it is difficult to find reasonable first control strategy.
Dynamic programing scales bad when the state space increases, for example continuous world.

Algorithm: Based on arbitrary fixed network topology where each weight w_1..n has a corresponding population p_1..n with m numbers. The values way be represented by a matrix x_ij, i=1..n, j=1..m.
First step: Build a genome from for value j, (x_1j, x_2j, ..., x_nj), and evaluate them. 
Second step: The quarter with best performance are recombined with crossover and mutation, replacing the worst performing.
Third step: The fitness of a genome determines the probability that it's values are permuted inside their corresponding sub populations. A well performing genome is less likely to permuted, but the behaviour of the probability function may be customised in detail.
The researchers permuted every value in step three, but suggested that it might not be efficient for larger networks.

Described other methods and compared benchmark, but did not compare them to CoSyNE in theoretical analysis.

#### Comments
Seems like it may be an efficient algorithm for where it is suitable. 
Probable disadvantage to NEAT:
 - Fixed topology
 - No speciation, may not handle different strategies in parallel
In an exhaustive setting for NEAT, could this method be part of the evolution of a specific species. In that local usage, to find weights, the topology is fixed. Is it still prob

### Efficient non-linear control through neuroevolution
Also discusses CoSyNE and the pole balancing problem
cited Evolving Neural Networks through augmenting topologies

### Incremental Evolution of Complex General Behavior
Different domain but might have some good thoughts in general.
Gabriel: Incremental evolution in some way is necessary to obtain complex behaviour.



# Probably relevant papers
### Neural Modularity Helps Organisms Evolve to Learn New Skills without Forgetting Old Skills
Might be relevant in an incremental training process. Dependant on stimuli, nodes take on different resistance to change.
cited Evolving Neural Networks through augmenting topologies

### An application of Reinforcement Learning to Aerobatic Helicopter Flight
This problem may share the same kind of unprocessed output data as we hope to use

### Behaviour transfer for value-function-based reinforcement learning
Might have some insights to how reinforcement learning and ANN works

### Evolutionary Function Approximation for Reinforcement Learning
Uses a combination of NEAT and Q-learning. Interesting to see how they relate Q-learning to NEAT
cited Evolving Neural Networks through augmenting topologies

### Opposition-Based Learning: A New Scheme for Machine Intelligence
"Possibilities for extensions of existing learning algorithms are discussed"



# Relevant if we modifies the problem
### Playing Atari with Deep Reinforcement Learning
Uses ai to forecast a value for different alternatives.

### Reinforcement Learning of Local Shape in the Game of Go
Fragments a large problem in many parts