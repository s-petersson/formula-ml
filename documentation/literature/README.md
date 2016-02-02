# Literature

## Papers

#### Introduction to the Theory of Neural Computation
(Martin)
Introductory paper to neural networks.

#### Evolving Neural Networks through augmenting topologies
(Martin)
This is the paper upon which MarI/O is based.

*So why waste valuable effort permuting over different topologies?*
````
The answers provided thus far are inconclusive. Some have argued that network
complexity can affect the speed and accuracy of learning (Zhang andMuhlenbein,1993).
Although this assertion is true for the backpropagation algorithm, it is not clear
whether it applies when weights are being optimized by evolution and not backpropagation.
````

An argument for using the method that applies to us.
````
A persuasive argument for the evolution of both topology and weights was put
forward by Gruau et al. (1996), who claimed that evolving structure saves the time
wasted by humans trying to decide on the topology of networks for a particular NE
problem.
````
The central idea.
````
We present a novel NE method called NeuroEvolution of Augmenting Topologies
(NEAT) that is designed to take advantage of structure as a way of minimizing the
dimensionality of the search space of connection weights. If structure is evolved such
that topologies are minimized and grown incrementally, significant gains in learning
speed result. Improved efficiency results from topologies being minimized throughout
evolution, rather than only at the very end.
````

#### Game AI: Simulating Car Racing Game by Applying Pathfinding Algorithms
(Gabriel)
Jung-Ying Wang and Yong-Bin Lin
International Journal of Machine Learning and Computing, Vol. 2, No. 1, February 2012
````
Not relevant. They used a modified A* algorithm but did not appear to focus on lap times. They found gates to drive the shortest possible path.

#### An AI tool: Generating paths for racing game
(Gabriel)
Charlie Irawan Tan, Chan-Min Chen, Wen-Kai Tai, Shi-Jim Yen
````
Uses a modified version of A*. Used in a horse racing game.
Probably not relevant. Not read yet. 

## Books

#### Neural Networks - A Comprehensive Foundation
(Martin)
Course literature for the Artificial Neural Networks course at Chalmers.

#### Artificial intelligence: approaches, tools, and applications
(Gabriel)
Gordon, Brend M
2011
Found at: Chalmers Library Online
Comment: Not read yet, but might be a good book to look in to.

#### Machine learning
(Gabriel)
Tom M. Mitchel
International edition 1997

A definition of machine learning, page 2:
>"A computer program is said to **learn** from experience _E_ with respect to some class of tasks _T_ and performance mesure _P_, if its performance at tasks in _T_, as measured by _P_, improves with experience _E_."

About neural network sizes, page 105:
All boolean functions can be represented with only one hidden layer. The number on nodes in the hidden layer may in the worst case scenario be exponential in respect to the number of input values. 
All bounded continuous real functions can be represented by a neural network with one hidden layer of sigmond nodes and linear output nodes. The level of approximation depend on the function and the number of nodes in the hidden layer. (Cybenko 1989; Hornik et al. 1989)
All real functions can be represented with a network with two hidden layers of sigmond nodes and the output layer with linear nodes. (Cybenko 1989). Better approximation with larger number of nodes.


## Articles

#### The Racing Line + Driving Corners Fast
(Gabriel)
http://www.drivingfast.net/techniques/racing-line.htm
http://www.drivingfast.net/techniques/track-corner.htm
Easy guide for racing theory. Useful for discussing level of physically correctness and judgement of ai result.

#### The Physics of Racing
(Gabriel)
Brian Beckman, Reformatted 25 April 2008
A series of articles. Quite informal and somewhat disconnected structure. Possibly useful for future reference.

#### Game ai pro: Collected wisdom of game ai professionals
(Gabriel)
Simon Tomlinson and Nic Melder
````
Probably not relevant. 
"Part VI Racing" present an overview of a possible archetectural design for a racing game at with multiple players. It covers track representation, race line for ai and tactical aspects.
They suggested the use of predifined splines (curves) as target for the ai drivers to follow. There may exist different lines for different strategies (e.g. optimal time or defensive). But, the articles did not explore ai to find the racing lines.