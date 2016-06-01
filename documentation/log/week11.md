# Week 11, March 28 - April 3
Less tempo due to Easter holy day

## Gabriel 
Read and commented paper: Accelerated Neural Evolution through Cooperatively Coevolved Synapses
Worked with the design of NeatCurveDataExperiment and visualisations.

Experimented with the parameters of the curve experiment. I found that lowering the amount of points decreased the training time. At the same time I increased the spacing of the points so that it would cover about the same part of the track. It also took longer to manage the whole track when the speed of the car increased. Some training data:
s15 d5200 t345 g32
s20 d2000 g20, d2734 g25, d5140 g52 
s25 d1000 g21
s25 d1109 d18, d2736 g29, d3105 g43 t4200, d5150 g46 t7200
s25 d5200 t434 g170
One of five trials at speed 25 succeeded, and the solution found was driving slower than the solution at speed 15. 

## Simon

### Distance driven algorithm
I solved the problem of the distance driven algorithm being incorrect. The problem was as following:
```
When a car passed a checkpoint c1, such that the line between the next checkpoint
c2 and c1 are not parallel, there is a specific area that the car can pass the checkpoint
c1 which results in the checkpoint counter of the car being increased and the distance
thus resulting in a smaller one than in the previous state.
```

The following image explains the problem:

![Image of distance problem](http://i.imgur.com/JBdJ4Fw.png)

The blue area is the area in which the bug occurs. The reasoning is that the closest point on either of the lines between the checkpoints (green dots and lines) will not be on the actual lines between the checkpoints. This makes for invalid calculations when the distance driven by the car is set as `distance to last checkpoint + distance from last checkpoint to car`, because the checkpoint is swapped "to early", and the car has actually not driven as far as to pass the next checkpoint yet.

I solved this problem by setting the distance driven by the car to the current checkpoints distance on the track, when the car is in the blue area. However this is not optimal since the cars distance will remain the same over all of the blue area. We have chosen to disregard this issue for now, and revisit the algorithm if we discover that this causes problems.

### Q-Learning
I read up on Q-Learning and the possibility of applying this to driving autonomous cars. There were some interesting results, however the main problem is that we do not have discrete actions (do this or do that), but continuous actions (do this, with a degree of X). This is not something that seems possible with Q-learning. However if we modify our problem, Q-Learning might be very relevant. There were some interesting approaches to utilizing supervised learning, based on generating sample data that we can backpropagate through the network.

Q-Learning is very different from the approach that we are currently using, and if there is no sight of progress from our current solution Q-Learning might be worth a shot. However, at the moment we feel that it is more beneficial to pursue the already implemented NEAT algorithm. Since that hasn't proven itself to be unsuccessful yet.

## Daniel
Finished the loading and saving of Generations, and the network can be loaded from an arbitrary location. Though there is still one bug, that is that the Generation count resets when a saved network is loaded.
One thing that I have come across in the research is Tile optimisation

## Martin

Fixed some issues in the code and implemented a simple neural network visualisation. 

Researched alternatives to NEAT and ways to use NEAT, did not find any promising ideas. 