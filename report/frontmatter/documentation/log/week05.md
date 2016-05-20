# Week 5, February 15-21
Summary: Most time went to implementation/reseach and preparation for next weeks first training algorithm. Simon and Gabriel worked on measuring distance travelled on the road, neccesary for the performance function, and input interpretation of the road as a grid. Martin and Daniel worked with implementations of neural networks. Martin also worked with a NEAT training algorithm.

### Concerning training algorithms
 - Hill climbing. Use result from simulation to score an AI instance. Stochastically take steps in different directions to increase the overall score. The direction of good steps may be repeated(?).
 - Hebbian learning, Hayken. During a network fire, if to adjacent synapses fire at the same time, increase their weight. If non fire, decrease weight. Back to the simulation as foundation for scoring. But if the simulation prove good behaviour, one might possibly go back in time to empower the behaviour for the previous input.
 
 
### How to get a line from the triangle track
Gabriel and Simon
Want to: Find dots in the middle of the road to build a line
Problem: 1) How to find dots? 2) How to order them?
The track is build up by connected triangulated quads. It is difficult to know for certain the ordering of the vertexes or in which direction the quads are triangulated.
 
 1a) Dots can be found on the middle on the lines of each triangle, and can be defined by the two vertexes it is in the middle of.
 1b) The line, two vertexes, and the dot in the middle them are common for two triangles
 
 2a) When one triangle has been used to build a segment, it can be discarded. The next triangle containing the shared dot will inevitable lead to the next dot in correct order.
 
 Algorithm
    - Add all possible pairs of vertexes in all triangles to a collection, a tree or similarly efficient structure. Always add pairs of vertexes in internal sorted order.
    - The second time a pair is added, add it to a second structure
    - The second structure contains two trees, the first one sorts on the first number, the second one on the second number.
    - Start with one vertex. Search the two structures for pairs containing that vertex, and that haven't been visited. The pair found will be describe the next dot, and give the next vertex to repeat the process on. Repeat.
    
 Note: if the quads change direction of triangulation, two pairs may be found during the last step. One of the found vertexes will not lead to a new undiscovered pair, use that first.
 Note: Watch over the fact that the algorithm may traverse the track backwards. 
 
 Complexity: If add/read operation with highest complexity is O(x), the whole algorithm will be O(nx). If hash lists are used, then x is amortised O(1).

### Gabriel
Helped Simon to find the algorithm to build a line out of the triangle structure of the track.

Implemented an algorithm that read the track and build a grid with information about the road ahead of the car. It iterates over the triangles, translates and rotates them to a frame of reference of the car, define a bounding box for the triangle, check all cells/points in the bounding box if they collide with the triangle and marks the cell as road if it do.

Simon an I also refactored the the structural parts of the simulation and test setup. The code is now more modular. For example the simulator now expects its users to define the important parts such as track, car, input processing to ai, output processing and the ai. This let the same Simulator implementation be used for different tests, with different implementations. Interfaces for example different parts of the ai may be needed later on.

Made a quick fix in the physic simulation. Multiplied the force capacity of the tyres by a constant. The car behaved much better now, more realistic steering capabilities and brakes faster than it accelerates.

### Simon
Made a refactorisation with Gabriel.

Planned and implemented an algorithm to take out checkpoints from the triangles representing the track. The checkpoints can then be used to build a line representing the middle of the track.
In order to write a fitness function that measure how far the car gets on the track, the distance driven is not relevant since the car could drive in zigzag. It must be related to the distance of the track.
The distance on the track sums the length of the line from start to the last checkpoint and the distance from the last checkpoint to the car. For a car that drives in the direction of the track, the function will always be increasing and make small jumps when a new last checkpoint is set.

### Martin

#### Fixed size neural networks.
Implemented a fixed size neural network. Using such a network does not seem very promising however, because the number of connections is so large. In order to optimise or train the network we need to optimise the goal function where all the weights are seen as inputs. If we have a neural net with several nodes in each layer, and a couple of layers, and with a couple of hundred inputs the total number of weights will probably be in the thousands. Optimising a function with that many parameters will be slow. Thus I didn't continue with implementing a training algorithm for that network and helped the others with the simulator instead. 

#### NEAT Algorithm & Implementation
Later I started looking at the source code to an implementation of the NEAT algorithm. One key thing that is really smart with the NEAT algorithm is the representation of the knowledge model. The key is that the actual neural network that does the calculation of the function is separated from the knowledge representation itself, i.e. the evolutionary algorithm works with a representation of the networks that is convenient to modify. The networks that are used in the calculations are then constructed from the instances in the evolutionary algorithm. This leads to a network implementation that is quite minimal. 

I also started to implement the algorithm in C++ based on the source code from the Super Mario bot. I simply started translating the lua script to a C++ equivalent without the domain specifics of that implementation. All concepts of the algorithm will be explained in detail in the source code, but it basically works as follows:

The NEAT algorithm is an evolutionary neural network machine learning algorithm that is based on evolving the topology of the network as well as the weights within the network. 

The algorithms works by applying slight mutations to a large population of Genomes, removing the ones that perform the worst. A genome can be seen as a specific DNA, or one individual or one neural network. A genome is basically a set of genes. A gene is a neural connection, an edge in the neural network. The actual nodes in the network are derived from the edges, ie if we have and edge from A to B we know that the nodes A and B must exist. 

### Daniel 
Been working on an implementation of a mutating directed  graph, to be used later on for mutations. The implementation makes sure that there is no cycles in the graph, wich could be devestating for recursive computation through the graph every time a "fire" event happens in the simulation/training.