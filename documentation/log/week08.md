# Week 8, March 6 - 13

Something seemed weird with the distance calculation. If the progress timeout was increased, the performance of the trained network increased. 

## Gabriel

### Notes: Evolving Neural Networks through Augmenting Topologies
In section 6, the paper mentions a concept called Incremental Evolution. It is about increasing the difficulty of the task as the ai evolve. Neuro evolution is likely to get stuck on a local optimum if the problem is too hard. If one let is solve a simple problem first, that will probably produce a good starting ground for the more complex problem.

## Simon

### Revisited distance driven
We have noticed during our training sessions that the distance driven by the car is not consistent between the visualisation and the AI trainer. I started investigating the reasoning behind this behaviour. The current implementation to check whether a simulation should continue running or not, is dependent on the distance driven by the car as well as a "progress timeout". Meaning that if the car has not made progress within this time, the simulator is reset and the evaluation of that network is finished. I noticed that the progress timeout is the reason behind the inconsistent behaviour, and thus I've started working on new ways to check whether the simulation should continue or not.

## Martin
Finished up the GUI-rendering used in debugging. 