# Week 7, February 29 - March 6


## Gabriel

### Thought concerning distribution of the NEAT algorithm
Instead of locks in the threading of evaluate, test and swap operation could be used instead. It is more efficient, but it might not make any relevant difference. It could be done by letting all jobs lie in an array. A variable may then denote how many elements have been taken by the workers, the one used for the synchronising test and swap. A worker try to increase the value for how many elements it wants to take.

If we implement a simple distributed system to increase calculation power, we need to consider the extra delay network cause. A worker should cash jobs, somewhat equal to the amount it finish during the round trip time. As the job list is close to finish, the amount of jobs a worker take upon should decrease gradually to 1, so that no worker sit with a list and some else do nothing. 


### Time in fitness function
Currently we want to have two purposes for the fitness function. First is to measure the distance the car manages to go and secondly to measure the speed. If the speed reaches the minimal possible, then the car drives in perfect racing lines.

Before the car manages to go all the way around, it is probably not good to reward it for speed, since it may reward a too aggressive behaviour, for example not braking in a curve. The speed is therefore only considered when the car has reached the end of the track. At that point, the following function is used:
fitness(distance, time) = distance + c * (sqrt(time_max) - sqrt(time))^2
where time_max denote the minimum time relevantly possible for the current track, and c is an arbitrary constant to resemble the size of distance. The function time term of the equation will grow as the time decreases, and have greater resolution as the time get closer to zero.


### Track curve
Implemented two indata preprocessor that describe the track as a curve. 
 - The first one more or less just take the checkpoints of the road. It writes pairs of distance and angle compared to the last point decribed.
 - The second one describes points on the mid line with fixed distances. As the algorithm traverse the mid line and finds the next point that should be described, it takes the direction from the last found point. It compares it to the direction found analogously for the last point and calculates the angle between them. That angle describes the point. Easier to understand with a figure.

### Readning notes: Heykin
One often want the network to be able to handle invariants of a class. The structure of the network may impose the same result different invariants. Example from image processing. The same recognition pattern structure is repeated, translated and rotated. [Comment, is this not what convolutional neural net is about?]

Two types of stored knowledge, page 57: Declarative: knowledge defined with symbols. Procedural: The "knowledge" is embedded in the executable code.

Dynamic programming is often used in conjecture with reinforcement learning in the realm of Markov Decision Problems. Q-learning also relate MDP.
  
### Reading notes: Evolving Neural Networks through augmenting topologies
NEAT uses a direct mapping between genome and phenotype. +Simplicity, -Size. Indirect mappings may be more compact, but may bias the topology in unpredicted ways. 
NEAT is able to determine gene homology, that two genes are the same for two different genomes. This enable crossover breeding without loosing features due to invariants.
NEAT separates different types of genomes in different species. Individuals only compete internally in a species. This make the possibility for invention survive a temporary decrease in fitness. [Comment: How much is it able to evolve without increasing fitness? May it need specific assistance in any case?]

NEAT starts with a minimalistic structure. It optimises and increase in complexity as the solution progress. This lead to a simpler solution that is faster to find than many other approaches. It is therefore not as necessary to penalise network size in the fitness function.


## Daniel

### Halftime presentation

We managed to do a decent halftime presentation in my oppinion. We got the question "How do you program this?" after the presentation, and that is about it. This is quite interesting as this tells us that the audience might not have understood everything as clear as we thought they would. 


We went over a the racing domain as well as how reinforcement learning works, but I think that the audience thought everything to be to abstract to get a good understanding of. It is a complex subject that we do adress, but the report need to address this problem that we realised.

## Simon

### Revisited distance driven
I implemented multithreading into the training simulator, since the training sessions started to require more computation power. I also implemented a way of reseting the simulator to its initial state, since creating a new simulator for every evaluation of a network, like we have done thus far, requires a lot of time. The evaluation of a network is now significantly faster than previously.

## Martin 

### Halftime presentation
Worked on the halftime presentation, which in my opinion went okay. It was quite difficult because our topic is quite hard to explain in such a short amount of time, and thus if we wanted to explain what we are doing and why we had almost no time to explain how it all works. 

### GUI
I started working on a GUI system which will be used to display debug information from the simulator. The functionality includes drawing rectangles, lines and texts. The idea is also that we can visualise the neural networks with the system, so that we can understand what the training algorithm is doing and how particular decisions are made within a network. 
