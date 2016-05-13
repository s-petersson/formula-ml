# Week 14, April 25 - May 1


## Daniel
Worked on saving statistics from the training


## Simon
Run experiments to learn about the abilities of neat.
Experimented with the stochastic parameters in neat. The networks seemed to add way to many nodes. After a certain point, it did not find new behaviour.


## Martin

### Fitness function
I tried to introduce speed or time into the fitness function to make the car disregard slow behaviours. The problem is that the goal of reducing time in different ways only makes the car try to crash as early as possible, which of course is not the desired behaviour.

Just for the sake of it I tried to add the distance driven to the fitness. We have used the distance along the track for a long time. When trying to maximise the distance driven it quickly learns to drive in a serpentine pattern to get as much distance as possible.

A possible solution to the problem of trying to maximise distance and minimise time, two goals which can be conflicting, is to only minimise the time. However, if we only minimise time, the car cannot have the ability to crash and thus stop the timer. IF that is the case, the algorithm will quickly find the behaviour to just steer hard to the side and crash as soon as possible. If we change the rules and say that the car cannot crash, if it does

## Gabriel
A large portion of the time went to test the different experiments, to better understand what type of results might be expected, and how they may be executed. I did not do this in a particularly planned matter, which may have made it slightly in efficient.

Also wrote some on the report, to prepare for the supervision meeting with Broberg next week.

### Shortest path experiment
When it only had to train for one output variable, it seemed to come a long way  closer to the optimal solution. It drivs perfect in some areas of the map, typically with the most curvature, often a bit less on places with less curves.

I tested today with a rather short length of the curve data. It only created edges to distance and angle to mid line, and the angle to the mid line 34 meters away (maximum possible 135 meters, but should probably have been 5-600 meters to actually see the next turn) It could explain why it sticks somewhat more to the middle on kinda straight parts, and close to the edge only in tight corners. Might be difficult to distinguish the different straight sections from each other.

Over training. After a number of generations, I came in and started to observe a training session after it had gone a kinda good result already. At this point I only saw it add one edge to the bias input, two hidden neurons in between others, and added edges in the hidden layer. Although it did slightly better, I think it only tuned the tactic it already used. The large flaws remained. I think this makes it risk making new bold innovation harder, since it have to outperform the present tuning. It also might focus the population to the overtrained properties.


# Notes from Supervision with the Library
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