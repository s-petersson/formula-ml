# Week 15, April 25 - May 1

## Simon
I ran a lot of experiments in order to examine the abilities of NEAT. There was a lot of time spent testing different settings and parameters. One example of what I was trying to fix, was the NEAT was generating a lot of hidden nodes. This was deemed unneccessary, since a lot of the time the problems that were solved by adding another node, could have been solved by simply modifying a weight in the network instead. This would have made the networks smaller and easier to reason about.

The settings that I based my modifications on were taken from the Super Mario bot implementation of NEAT as well as the configurations explained in the original paper. I made a few modifications, the most notable of which was the reduced probability of adding another node. However I also experimented with parameters such as the population size and stagnation rate of networks. (The time that they are allowed to survive during training, before NEAT kills them of)


## Martin

### Fitness function
I tried to introduce speed or time into the fitness function to make the car disregard slow behaviours. The problem is that the goal of reducing time in different ways only makes the car try to crash as early as possible, which of course is not the desired behaviour.

Just for the sake of it I tried to add the distance driven to the fitness. We have used the distance along the track for a long time. When trying to maximise the distance driven it quickly learns to drive in a serpentine pattern to get as much distance as possible.

A possible solution to the problem of trying to maximise distance and minimise time, two goals which can be conflicting, is to only minimise the time. However, if we only minimise time, the car cannot have the ability to crash and thus stop the timer. If that is the case, the algorithm will quickly find the behaviour to just steer hard to the side and crash as soon as possible. If we change the rules and say that the car cannot crash, we can just optimise on time, since the car will complete the circuit during every evaluation. When the car crashes it is lifted back onto the track and can continue.

However, it is complicated to decide how much the genome should be punished for crashing. It should always be better to avoid crashes, but at the same time the training should not force the car to drive slowly.

This training process seems to work, the genomes improve, but it does not seem to be significantly better than the other process, additionally it takes longer time since each genome drives around the whole track instead of being terminated at the first crash. 

### Image saving

I wrote a system that given a neural network simulates how it drives around the track, then renders the track and the race line to an image and saves it to the disk. The speed of the car is saved using the colour of the race line. This procedure is called each time an improvement is found, thus it enables us to view how the system drives at each improvement. This is very useful when running long training sessions. You can review how the driving behaviour progressed.

### Turning radius

Added the rendering of a curve that displays the cars current turning radius as an arc at the cars position. This gives us the ability to see whether or not the car should be able to complete a corner at the current speed. Since the size of the arc depends on the current speed it also indicates the speed of the car. 


## Gabriel
A large portion of the time went to test the different experiments, to better understand what type of results might be expected, and how they may be executed. I did not do this in a particularly planned matter, which may have made it slightly in efficient.

Also wrote some on the report, to prepare for the supervision meeting with Broberg next week.

### Shortest path experiment
When it only had to train for one output variable, it seemed to come a long way  closer to the optimal solution. It drivs perfect in some areas of the map, typically with the most curvature, often a bit less on places with less curves.

I tested today with a rather short length of the curve data. It only created edges to distance and angle to mid line, and the angle to the mid line 34 meters away (maximum possible 135 meters, but should probably have been 5-600 meters to actually see the next turn) It could explain why it sticks somewhat more to the middle on kinda straight parts, and close to the edge only in tight corners. Might be difficult to distinguish the different straight sections from each other.

Over training. After a number of generations, I came in and started to observe a training session after it had gone a kinda good result already. At this point I only saw it add one edge to the bias input, two hidden neurons in between others, and added edges in the hidden layer. Although it did slightly better, I think it only tuned the tactic it already used. The large flaws remained. I think this makes it risk making new bold innovation harder, since it have to outperform the present tuning. It also might focus the population to the overtrained properties.

## Daniel
I've worked on saving data to .csv file regarding all genomes in each generation in order to see what happens in every generation were the fitness is not increased. Unfortunally, the saving was not able to be asyncrouns within a reasonable amount of time spent coding. Still it yielded some really interesting results regarding overfittng

###Overfitting
When running training sessions for a long period of time at the "Full Controll" experiment, the session stopped improving. By the data gathered from the session, it was evident that it could not reach an increase in fitness. Half of the genomes hade a fitness of the best recieved fitness, and the other half performed just above lowest achievable fitness. This was probably due to that all but one species was removed, and that the best genome have breeded to fill "itself" to be in half the species. When a the new, other half of mutated genomes, were eveluated, the fragile structure of the "original" genome were destroyed to make it crash right away. As they were still the same species, the worst half were removed to just repeat this process, getting stuck in a local maxima, and never reaching a more optimal structure.

Progressive fitness function, as well as keeping the species alive longer would be key to achive our wanted behaviour. 


### Notes from Supervision with the Library
If the wanted behavoir is reached reagrding racing lines and strategies, then resources for these are not that needed. We do not need that if we dont cover it.

Scopus/Web of science. Try all the different databases/searchengines, the results may differ.
These have nice ways to do citeation seaches.

Supervisor searched for racing, car, racing line in Google schoolar, and it gave som nice results, and this was more citeationsearches. E.g. : "race line", "Machine learning"
Also search for IEEE for conference papers.

Find sources from different kinds of context.

Uppsattser.se
Studentarbeten på chalmers, found at biblioteket under "Tjänster"

Look for TORCS, there are a lot of things written for it, and there might aswell be useful sections in these text.

If there is an "unpublished" paper that is still on a webpage, it can be used as a web source. 


