# Week 6, February 22-28
During the supervision meeting Prasad wanted us to focus more on results. This inspired us to start simple and to aim for the car to just follow the mid line.


### Report Lecture

We need to decide on which format we want to use in the report. For instance IMRD, ILMRDC etc. We should check with Prasad and especially our examinator which format is preferred for us. 

There exist a document "skrivhänvisningar" on the web page of "Fackspråk". The different sections named are not necessarily sections, but functions of the text. Figures have to contribute with something and do not copy content from figures to the text, comment it. Parts in a comment: reference, content, focus.



### Gabriel
Worked with code to get the angle and distance of the car to the middle line. But Simon and Martin helped to get it to work.

#### Exhaustive search
Made a simple exhaustive search algorithm to try out different weights and choose the best using the simulator.
 - input: distance & angle to mid line
 - hidden: two nodes, one layer.
 - output: steering. It drives with constant speed.
At a low speed it could drive in the middle of the track, but got stuck at the chicane, since it required more planning with the current performance and speed of the car.
Based on the two input values the following general behaviour is desired:
Position compared to mid line  | angle to mid line (positive = left of)
Right | Negative: steer to the left
Left  | Positive: steer to the right
Stabilizing manouvers:
Right | Positive: steer slightly to the right
Left | Negative: steer slightly to the left

The weights was in the range [-3, 3] and the sigmoind function [-1, 1]

Cheated on the structure. Both of the inputs to one of the hidden nodes shared the same weight, the other hidden node had a defined 1 to the angle and 0 to length.

#### Reeding Heykins
How to build in prior knowledge to the knowledge model
  - Manually define some aspects of the topology to decrease the number of synapses. It do not have to be a complete network. My thought: 1. Neurons in the first hidden layer might only take synapses from data points related geographically. 2. The areas covered may overlap. 3. The synapses may have non-random starting values
  - Let several synapses share the same weight, limit the number of free variables. My thought: 1. The first computation for related data point may possibly share the same weights, or a function operating on the same weight that slightly decrease the weight for more distant data points. 2. The wights may be symmetric for right and left. 3. Feedback from the group: this is the basics in some image recognition networks.
  
  
### Martin

#### NEAT Implementation.
This week we made a lot of progress on the NEAT-algorithm Implementation. I did a refactorisation of the implementation to make it more manageable and modularised. This will make it easier for us to improve individual parts of the implementation when needed. 

I also started to use the implementation for different purposes, one was to check whether it worked or not by making it approximate XOR. However even though the algorithm seems to do the right things it only manages to learn 3 out of 4 cases of XOR, thus something is amiss. One possible solution is to disallow recurrent links in the network and use a more advanced basic network. Right now the Genomes are initialised with no links and are then mutated. When the authors to the NEAT paper tested the algorithm with XOR they connected all the inputs and the bias to the output with random weights (i.e. 3 predefined edges). This might yield better results. 

We also connected a network trained with the NEAT algorithm to the simulator and achieved a result. We trained the network to steer based on the cars relative position on the road. If the car is on the right side of the road it steers to the left, and vice versa. The car behaviour correctly according to that behaviour however the behaviour in itself is quite unstable. Because the car wants to steer toward the centre of the track it arrived at the centre line at a steeper and steeper angle, and thus it starts to drive in a sinusoid manner along the track.

A proposed solution that originated from Gabriel was to use both the cars position and angle relative to the track. The goal behaviour is then to steer towards the centre of the track if the car is far from the centre, and to align its direction with the track so that it does not drive sideways. 

### Simon 
Spent most of my time refactoring the code in a way so that we can separate our different methods of implementing the AI into "experiments". This gives us an easy way of implementing several different methods for solving the problem, without having to remove the old tries.

### Daniel
Found paper (Adaptive representations for reinforcement learning) going over diffrent types of NEAT, and similar algorithm. In this paper, our implementation is calles fs-NEAT, and means that we start without edges from each input. Normal NEAT does have at least one edge from each input node according to this paper, and that is not what we have done in our neat implementation so far.

I also prepared for the mid-term presentation
