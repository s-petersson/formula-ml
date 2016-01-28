# Literature

## Papers

#### Introduction to the Theory of Neural Computation
Introductory paper to neural networks.

#### Evolving Neural Networks through augmenting topologies
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


## Books

#### Neural Networks - A Comprehensive Foundation
Course literature for the Artificial Neural Networks course at Chalmers.

#### Artificial intelligence: approaches, tools, and applications
(Gabriel)
Gordon, Brend M
2011
Found at: Chalmers Library Online
Comment: Not read yet, but might be a good book to look in to.

## Articles

#### The Racing Ling + Driving Corners Fast
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