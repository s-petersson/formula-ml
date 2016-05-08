# Week 14, April 18 - 24
Martin and Simon spent a lot of time on fixing bugs. Most of the issues came from the refactor Gabriel made, but it was not clear why all of the errors occurred.

## Gabriel
### Multiple/Alternating track experiment
A research Prasad, our supervisor, heard of and a paper we found suggested that alternating the task in a particular may give modularised networks. Our problem is different in nature, but it might be interesting to test the alternating of tasks. Might provoke the networks to stay more general and find better innovation

Another benefit is that in a long track, subsequent curves depend on succeeding on previous curves, but testing with several ease that dependence.

So I implemented code to run two different experimentsm evaluating several tracks all the time, or alternate between them.

### What to think about while testing, ideas
Is driving slow a problem. Do we need to give a penalty for driving slow?

## Martin

### Car trail rendering
Implemented a visualisation of the racing line of the car as a curve with the colour varying with the speed of the car. 

### Bug fixing
A refactorisation of the code introduced a couple of issues to the code which we solved. Some were caused by illegal memory accesses and some were caused by a synchronisation issue where several threads could call the same lambda function at the same time.  