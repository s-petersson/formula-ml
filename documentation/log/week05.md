# Week 5, February 15-21
Concerning training algorithms
 - Hill climbing. Use result from simulation to score an AI instance. Stochastically take steps in different directions to increase the overall score. The direction of good steps may be repeated(?).
 - Hebbian learning, Hayken. During a network fire, if to adjacent synapses fire at the same time, increase their weight. If non fire, decrease weight. Back to the simulation as foundation for scoring. But if the simulation prove good behaviour, one might possibly go back in time to empower the behaviour for the previous input.
 
 
## How to get a line from the triangle track
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
 
 Complexity: If add/read is log n, the whole algorithm will be n log n.
    