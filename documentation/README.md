#TODO

## Think
Can we reduce other problems to the racing problem? 


## Experiments
### Supportive features
- Instead of killing the simulation when it crashes, give it a penalty but let it continue? Reset the position?
- Can we give the car some kind of warning? E.g. a dangerous curve ahead.
- Test suits
  - Test for longer periods of time. 
  - Test multiple times

### General test procedure
X Test for longer periods of time
X Enable acceleration. 
- Multiple short track tests. Failure in one, let it still do and progress in subsequent turns. Be cautious of parameters, so that it is not beneficial to ignore some part.
- Alternating multiple short track tests. May produce modular networks?

### Behaviour specific 
X One or two curves only
- Narrow track with brake enabled: Small variation of possible racing lines, focus on achieving good braking.
- Find shortest path / Search for better racing lines: Fixed speed? Count fitness to a good racing line? Give a good racing line as in data?

### Indata
- Add grid as it is today
- Grid with "Convolutional neural network" features

### Analysis
- Poke around with networks manually

## Optimisation
- Quad tree track


## Help features
- Debug features    
  - Physics info


## Other 
- Refactor Experiments (Gabriel)


## Individual
### Daniel

### Gabriel
- Refactor Experiments (Gabriel)

### Martin

### Simon




# Ideas for future
- Q-learning
- Tile
- Heuristics fitness, current ideas: some crashes before other
- Alternative fitness functions in NEAT, different heuristics 
- Use of multiple networks for different purposes. Feed in to each other?
