# Week 14, April 18 - 24

## Gabriel
### What to think about while testing, ideas
Is driving slow a problem. Do we need to give a penalty for driving slow?



## Martin

### Car trail rendering
Implemented a visualisation of the racing line of the car as a curve with the colour varying with the speed of the car. 

### Bug fixing
A refactorisation of the code introduced a couple of issues to the code which we solved. Some were caused by illegal memory accesses and some were caused by a synchronisation issue where several threads could call the same lambda function at the same time.  