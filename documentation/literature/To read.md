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

### Efficient Reinforcement Learning through Evolving Neural Network Topologies
(Simon)
Discusses difference in efficiency between evolving a network with fixed topology and evolving one without fixed topology.

### Deep learning in neural networks: An overview
(Simon)
Hot paper & Highly cited
Review of a number of different algorithms
cited Evolving Neural Networks through augmenting topologies

### Combining NEAT and PSO for learning tactical human behavior
Probably discusses relevant problems concerning planning and strategy! Discusses an algorithm called PIGEON.
cited Evolving Neural Networks through augmenting topologies

### Incremental Evolution of Complex General Behavior
Different domain but might have some good thoughts in general.
Gabriel: Incremental evolution in some way is necessary to obtain complex behaviour.

### Neural Modularity Helps Organisms Evolve to Learn New Skills without Forgetting Old Skills
Might be relevant in an incremental training process. In this method one change the resistance of nodes to change. This supposedly lead to faster learning of new tasks and a preservation of old learned knowledge.




# Probably relevant papers

### Reinforcement Learning Neural Network to the Problem of Autonomous Mobile Robot Obstacle Avoidance
http://www.ice.ci.ritsumei.ac.jp/~ruck/CLASSES/INTELISYS/NN-Q.pdf
Discusses the use of reinforcement learning neural networks for controlling an autonomous vehicle such that it can move around freely in a space and avoid obstacles that are in its way.

### Autonomous Helicopter Control using Reinforcement Learning Policy Search Methods
http://repository.cmu.edu/cgi/viewcontent.cgi?article=1082&context=robotics
Discusses the use of neural networks and reinforcement learning used to control a helicopter in flight.

### Autonomous inverted helicopter flight via reinforcement learning
http://www.robotics.stanford.edu/~ang/papers/iser04-invertedflight.pdf
Discusses the use of neural networks and reinforcement learning used to control a helicopter in inverted flight (upside-down).

### Reinforcement Learning Using Neural Networks, with Applications to Motor Control
http://www.remi-coulom.fr/Publications/Thesis.pdf
Walks through reinforcement learning with regards to neural networks and tries to apply this to building controllers for motor tasks such as driving a car, swimming or walking.
*NOTE:* They discuss the actual task of finding the quickest way for driving a racecar around a track. However I quickly glanced over the article and it wasn't very in-depth. Thus it might not be extremely relevant, however still a good read.

### Neural Modularity Helps Organisms Evolve to Learn New Skills without Forgetting Old Skills
Might be relevant in an incremental training process. Dependant on stimuli, nodes take on different resistance to change.
cited Evolving Neural Networks through augmenting topologies

### Introduction to Evolutionary Algorithms
A book

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
/
### Reinforcement Learning of Local Shape in the Game of Go
Fragments a large problem in many parts
