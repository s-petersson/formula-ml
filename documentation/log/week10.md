# Week 9, March 21 - 27


## Martin

### Alternative Circuits
Gabriel had read that the order in which different problems are introduced to the training algorithm can be relevant for the success and speed of the training. The basic concept is that it is more likely that a small change occurs that leads to better results in a simple problem rather than a change that solves a more complex problem. For instance in our case it is easier to find a network that is able to take a simple corner than to find a network that is able to take a corner where planning and the correct positioning is required in order to succeed. 

However the first circuit we used, Spa, was not really suitable in that regard. In the Spa circuit there is a chicane early on, which can be quite hard to get through, thus we decided to try another circuit instead and see if it made any difference. 

I created a circuit with the intention that the difficulty of the corners should be increasing as you get further along the track. Thus the corners in the beginning are quite gentle, then there are some sharp single turns before the chicanes and pinhead corners. I also found the Formula 1 federations circuit requirements. The most relevant things were that the track should always be at least 12 meters across, however at the starting grid into the first corner the minimum is 15 meters. The circuit is modelled in a way that allows for regenerating the polygon mesh, thus we can also test the same circuit with varying track widths if it is interesting. If I recall correctly the current track width is 20 meters. 


## Daniel 
Finished the ability to save networks, but just a prototype where the path is absolute and not changeable. It have taken me more time than I thought it would, and that is because that I am not quite familiar with C++ yet. I thought I was done with loading a network as well, but I misunderstood some of the I/O in C++, but that will be fixed the next week.

## Simon
While trying to figure out new ways to figure out when to terminate the simulation I discovered that the distance driven algorithm that is currently in place is in fact incorrect. More specifically when the car drives over a set of checkpoints, at a specific area on each checkpoints the distance driven decreases relative to the last distance driven.

This should in fact never happen, since the car cannot move backwards nor can it move to the side without moving forward at the same time. Thus the distance driven should ALWAYS be increased every simulation update.

My week has been spent trying to solve this problem. I have spent a lot of time going over the implementation in order to make sure that it is correct. It seems to be correct. This leads me to believe that something is wrong with the general thought process of how the distance driven is calculated.


## Gabriel

### Finished: Refactoring NeatTrainer
Contents was moved to the Experiment class NeatCurveDataExperiment.
Now, NeatTrainer require the user to provide an evaluating function, thus not have any direct dependency on the simulation. In detail, NeatTrainer require a factory that produce objects with the virtual method evaluate. This abstraction let 1) NeatTrainer create one instance of the object/function for each thread 2) the object to be a memory location for resources reused by the evaluating function.
This pushes some responsibility of compiling functionality to an upper layer in the program, NeatCurveDataExperiment. The version today works, but may benefit of an improved design, to improve the useability of features in that layer.
