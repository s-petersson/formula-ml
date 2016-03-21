# Literature

## Papers

### The 2009 Simulated Car Racing Championship
Presents the competition and the methods used by the top 5 performing teams
Team C cited Evolving Neural Networks through Augmenting Topologies

Notes:
They used an open source racing simulator called TORCS, C++. Even if TORCS seemed to be an open platform, this was limited in the purpose of the competition to a set of sensor data.


Team A: Had six different controllers; gear control, target speed, speed control, steering control, learning, opponent management. Learning updated knowledge on speed for different sections from experience, target speed module instructed the speed control.
[Comment, Gabriel: Team A approach to max speed seemed very simple and straight forward. Building it in to a function that base the result on solely sensor data seems more complex.]


Team B: Used a function that determined the speed and target angle out of sensor data, but took also experience in to account.


Team C: NEAT with scripted behaviour for start, straights, crash-recovery, gear change and overtaking. During the straights, the car was forced to accelerate as much as possible. Focused on inputs that correlates to driving actions: speed and sensor data. Two outputs: steering and brake/acc. 

The developer says that a strength of this approach is that it demands very little domain knowledge. It also seemed to produce more general behaviour than other approaches, it managed to learn from only driving at one track. The track he trained at was the track with presented most of the interesting features in the game.

He only managed to evolve careful ai, that mostly drove at the center of the track and was unable to get optimal racing lines.

[Comment, Gabriel: Is out-of-the-box NEAT to narrow in its search to find out efficient solutions in this domain? Bare in mind that it is very easy to find a network that stays on the middle of the road.]


Team D: Used a state machine, to remember what type of segment the car is in or if it is about to do a specific action such as overtaking and a location to write behaviour code, a fuzzy logic module, to identify/handle transitions between states, and a classifier, to identify the shape and properties of the track. They then used evolutionary algorithms to train the fuzzy logic module.

The fuzzy module seldom gave full gas or brake outputs. The developer said that fuzzy module originally was developed for autonomous vehicles, that should drive safe, but would investigate wither it is possible to tweak it.


Team E: Used a evolutionary algorithm for speed control. Experience showed that it gave a more robust solution when training on a track with a wide variety of track features.


Organisers comment: Participants that used imitation based learning did not score well, and that is is congruent with a paper. It states that imitation learning generally fails for racing ai.



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

Accelerating or braking will cause the balance of the weight to transfer (Part 1)
In order to stay in a circular motion, the central acceleration a = v^2 / r (page 12). The ability to take a turn is very sensitive to the speed.
autocrossing: "The absolute fastest way to go through a corner is to be just over the limit near the exit, in a controlled slide." (page 13)
The racing line for a specific corner depends on the corners before and after. Small changes may have consequences for a long part of the following section. The best drivers figure out the line for the course as a whole. (page 13)
The optimal line for a single corner is late apex, which allow for an earlier acceleration for the following straight. (part 5, Racing lines, page 15) 
Air resistance/drag and rolling resistance are forces that must be overcome in order to keep constant speed (Part 6, Speed and Horsepower, page 18)
Air resistance F proportional to v^2, see formula in text (page 19 + other source)
Acceleration: Torque creates a backward force on the ground. Newtons third law, opposite force accelerates the car forward. (Part 7, Traction budget, page 22)
Maximum force F <= µW, where µ is friction constant, W is the weight load of the tyre (page 23)



#### Game ai pro: Collected wisdom of game ai professionals
(Gabriel)
Simon Tomlinson and Nic Melder
````
Probably not relevant. 
"Part VI Racing" present an overview of a possible archetectural design for a racing game at with multiple players. It covers track representation, race line for ai and tactical aspects.
They suggested the use of predifined splines (curves) as target for the ai drivers to follow. There may exist different lines for different strategies (e.g. optimal time or defensive). But, the articles did not explore ai to find the racing lines.