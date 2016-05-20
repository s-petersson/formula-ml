# Literature

## Papers

###Efficient Non-Linear Control through Neuroevolution
(Martin) 
http://nn.cs.utexas.edu/downloads/papers/gomez.ecml06.pdf
This paper show that neuroevolutionary methods, especially CoSyne, but also NEAT and SANE can be effective at finding non-linear control patterns. Also gives an overview of different relevant neuroevolutionary algorithms.

###Speciation in NEAT
(Martin) 
http://apps.cs.utexas.edu/tech_reports/reports/tr/TR-1972.pdf
Technical report covering how the speciation mechanisms in NEAT affect the algorithms performance. 
````
@techreport{nodine:ugthesis10,
title={Speciation in NEAT},
author={Timothy Nodine},
number={HR-10-06},
institution={Department of Computer Science, The University of Texas at Austin},
type={Undergraduate Honors Thesis},
url="http://nn.cs.utexas.edu/?nodine:ugthesis10",
year={2010}
}
````

### Reinforcement Learning Neural Network to the Problem of Autonomous Mobile Robot Obstacle Avoidance
Developes a robot that autonomously drives around in an environment with obstacles in it, without crashing into these obstacles. This is done using Q-Learning and representing/approximating the Q-function (or the Q-table) as a neural network.



### Neural Fitted Q Iteration - First Experiences with a Data Efficient Neural Reinforcement Learning Method
(Simon)
**NOTE:** One should have knowledge about `Markov Decision Processes` and `Q-Learning` before reading this summary.

This paper focusses on Q-learning modified to fit neural networks. The problem that it tries to solve is the following:
```
How can the positive effects of global multi-layer perceptrons (neural networks)
learning be utilized and at the same time avoid the negative aspects of them?
```
Meaning that they want to avoid the negative effects of "forgetting" something that the network has already learned, because it is trying to learn something else (It learns globally, not locally).

They solve this problem by applying Q-Learning to neural networks. Note that Q-learning applied to neural networks, from what I can conclude, means that the networks looks as following:
**Inputs:** Current state + Possible actions
**Hidden layers:** Anything you want really.
**Outputs:** One value. The Q-value.
The reasoning behind this is that the neural network will represent the Q-function. The Q-function looks as following `Q(s, a)`. It takes the state and an action, and returns a value after having performed that action to that state. We can then chose the action that gives us the highest value back from the Q-function for that state.

**The algorithm:** `Neural Fitted Q Iteration (or NFQ)`
The basic idea is the following: 
```
They collect a set of "experiences" by performing a random action `a` in a given
state `s` in order to achieve the next state `s'`. They save these experiences
into memory, such that they can then be used to perform *supervised learning*
based on theese experiences.
```
**NOTE:** They use something caled Rprop for supervised learning. Which they claim to be advanced supervised learning and to be very effective.

The algorithm consists of two major steps:

1. The generation of the training set P and
2. the training of these patterns within a multi-layer perceptron (neural network).

The input part of each training pattern consists of one state `s` and an action `a`. Theese will be the inputs supplied to the neural network. The target value that we want the neural network to compute will be the following:
```
c(s, a, s') , 			 		   if sl ∈ S+
C−, 							   if sl ∈ S−
c(sl, ul, sl) + γ*minb*Q_k(s', b), else (standard case)
```
where
`S+` = The set of goal states (Were we have performed the task correctly). Often simply a positive constant value.
`S-` = The set of forbidden states (Were we have failed performing the task).
`c` = The cost function of going from `s` to `s'` by performing `a`.
`C-` = Maximum output value that the neural network can provide.
`Q_k` = Current estimate of the Q-function (the neural network).

#### Comments
I do not yet know how the value that is received by the neural network, the "Q-value" can be used. I've read somewhere that we can determine what action to take in a certain state based on the highest or lowest Q-value received for all available actions (however we chose to set that up). But how to do this I am yet to understand.
Maybe reading this one more time will make things clearer: https://www.reddit.com/r/MachineLearning/comments/1kc8o7/understanding_qlearning_in_neural_networks/.

#### Notes
I think this can be useful if we manage to figure out exactly how to use it. The fact that we can use supervised learning by "generating data" is very interesting.



### Neural Modularity Helps Organisms Evolve to Learn New Skills without 
Forgetting Old Skills

#### Comments
Might be relevant in an incremental training process. In this method one change the resistance of nodes to change. This supposedly lead to faster learning of new tasks and a preservation of old learned knowledge. (Gabriel?)

Could be relevant if we decide to treat the different controls of the car as different tasks, ie. by training steering, braking and accelerating as different problems. -Martin.


#### Notes
The key conclusions of the paper is that modularity can reduce catastrophic forgetting when learning new tasks. For example if we have a network that should solve several tasks we can restrict the training to one module when we train the network to solve task 1, then when it should learn task 2, we only allow mutations in module 2. Furthermore the conclude that modularity can be achieved my imposing a cost on neural connections. 

>"Moreover,the modularity-inducing effects of adding a connection cost were shown to occur in a wide range of environments, suggesting that adding a selection pressure to reduce connection costs is a robust, general way to encourage modularity [23]."



### Accelerated neural evolution through cooperatively coevolved synapses (CoSyNE)
(Gabriel, 31/3)
published 2008
Describe a neuroevolution method CoSyNE and benchmark it to other reinforcement algorithms.
Cited and compared to NEAT.

#### Comments
Seems like it may be an efficient algorithm for where it is suitable. 
Probable disadvantage to NEAT:
 - Fixed topology
 - No speciation, may not handle different strategies in parallel
In an exhaustive setting for NEAT, could this method be part of the evolution of a specific species. In that local usage, to find weights, the topology is fixed. Is it still problematic that it does not handle different strategies?

#### Notes
Introduction: Problem targeted (1.) non-linear and noisy, difficult to engineer a models, (2.) and complex so it is difficult to find reasonable first control strategy.
Dynamic programing scales bad when the state space increases, for example continuous world.

Algorithm: Based on arbitrary fixed network topology where each weight w_1..n has a corresponding population p_1..n with m numbers. The values way be represented by a matrix x_ij, i=1..n, j=1..m.
First step: Build a genome from for value j, (x_1j, x_2j, ..., x_nj), and evaluate them. 
Second step: The quarter with best performance are recombined with crossover and mutation, replacing the worst performing.
Third step: The fitness of a genome determines the probability that it's values are permuted inside their corresponding sub populations. A well performing genome is less likely to permuted, but the behaviour of the probability function may be customised in detail.
The researchers permuted every value in step three, but suggested that it might not be efficient for larger networks.

Described other methods and compared benchmark, but did not compare them to CoSyNE in theoretical analysis.

#### Further reading: Efficient non-linear control through neuroevolution
Also discusses CoSyNE and the pole balancing problem
cited Evolving Neural Networks through augmenting topologies



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


##### Adaptive Representations for Reinforcement Learning
(Daniel)
Shimon Whiteson
Paper going through different types of reinforcement learning, including NEAT and fs-NEAT and what their differences can give.
A most relevant paper as it describes how to drive car around a track with fs-NEAT with a lot of in-data.



#### An AI tool: Generating paths for racing game
(Gabriel)
Charlie Irawan Tan, Chan-Min Chen, Wen-Kai Tai, Shi-Jim Yen
````
Uses a modified version of A*. Used in a horse racing game.
Probably not relevant. Not read yet. 


#### Game AI: Simulating Car Racing Game by Applying Pathfinding Algorithms
(Gabriel)
Jung-Ying Wang and Yong-Bin Lin
International Journal of Machine Learning and Computing, Vol. 2, No. 1, February 2012
````
Not relevant. They used a modified A* algorithm but did not appear to focus on lap times. They found gates to drive the shortest possible path.



## Books

#### Fast car physics
Chuck Edmondson
2011
Baltimore
Md. : Johns Hopkins University Press

##### Notes
Chapter 1:
Has some insight on acceleration and braking, good source of references!

Chapter 3: 
Physics:
traction circle. Mention the four different main forces: gravity, normal force, drag & tyre-road.
An isotropic distribution of forces in the plain is an assumption. Presented data from a real car sensor, it was oval looking. That car had more g capabilities laterally than forwardly. Braking more effective than accelerating p50
Accelerating is slower than braking. Formula 1 cars can brake with 4-5 g at high speed, but only about 2 g at lower speeds. p52
Different tyre designs will provide different distibutions of force capabilities in different directions, p53.

eq. 3.1 and 3.3 gives the formula f_c = mv^2/r, p55.

Race lines:
When the benefit of a race line in a (symmetric) curve is calculated, the author take the following aspects into account: Part before braking, braking section, turning section, acceleration section, following segment. Time/distance and speed calculated for each section. p56
Referencing Alan Johnson, 1971, Driving in Competition. Type 1 curve: Entrance to straights, most important. Type 2 curve: Exit after straight. Type 3 curve: Transition between type 1 and 2 curves. Type 1 has always priority! p 62
Better perforformance in type 1 curve will win a battle during the straight, even if one temporarily fall behind a little. Will accumulate more distance. p63
Taking type 1 curves: little entry speed (brake relatively much), late apex exit. Transition point early, early acceleration, more exit speed if done correctly. p63
Type 1 curves advanced: Optimal: balance at the traction limit with acceleration and steering, at the same time as other aspects as weight transfer play a role.
Taking type 2 curves: Late braking, early apex, late transition point, low exit speed. p70
Taking type 3: Not a single strategy, greater variation. Depend on many circumstances, car performance, slopes, other curves, etc. 
When optimising, plan backwards, as the entry to a curve put demands on the exit of the previous. E.g. Type 1 curves are dominant.



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