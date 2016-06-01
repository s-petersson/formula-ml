# Week 14, April 18 - 24
Martin and Simon spent a lot of time on fixing bugs. Most of the issues came from the refactor Gabriel made, but it was not clear why all of the errors occurred.

## Gabriel
### Multiple/Alternating track experiment
A research Prasad, our supervisor, heard of and a paper we found suggested that alternating the task in a particular may give modularised networks. Our problem is different in nature, but it might be interesting to test the alternating of tasks. Might provoke the networks to stay more general and find better innovation. It is also inspired by the paper: Neural Modularity Helps Organisms Evolve to Learn New Skills without Forgetting Old Skills, even if the problem is modelled differently

Another benefit is that in a long track, subsequent curves depend on succeeding on previous curves, but testing with several separately ease that dependence.

So I implemented code to run two different experiments evaluating several tracks all the time, or alternate between them.

### What to think about while testing, ideas
Is driving slow a problem. Do we need to give a penalty for driving slow?

## Martin

### Car trail rendering
Implemented a visualisation of the racing line of the car as a curve with the colour varying with the speed of the car. 

### Bug fixing
A refactorisation of the code introduced a couple of issues to the code which we solved. Some were caused by illegal memory accesses and some were caused by a synchronisation issue where several threads could call the same lambda function at the same time.

### FS-NEAT
Throughout the project we have used a variant of NEAT called FS-NEAT where the initial structure used in the networks is truly minimal. The difference is basically if the networks start with one connection or with connections from each input to each output. 

I implemented a flag in the algorithm that enables us to toggle this feature. We noticed that generating an initial structure does not lead to any improvement, most of the connections are immediately disabled.

### Tracks
Added some additional track models, one narrow version of the circuit and several shorter segments. That can be used to test the system on several track or just shorter segments. 