# Week 5, February 15-21
Concerning training algorithms
 - Hill climbing. Use result from simulation to score an AI instance. Stochastically take steps in different directions to increase the overall score. The direction of good steps may be repeated(?).
 - Hebbian learning, Hayken. During a network fire, if to adjacent synapses fire at the same time, increase their weight. If non fire, decrease weight. Back to the simulation as foundation for scoring. But if the simulation prove good behaviour, one might possibly go back in time to empower the behaviour for the previous input.

# Week 4, February 8-14
Daniel had a few days of vacation. Simon planned to, but did not. 

Martin added the feature to detection if a point is on the track.

### Planning report
Most of the work went to the planning report. 

Most of the theoretical reasoning is up to date in the report, and rather complete. Also see Theory section for this week

It took a while before we figured out what the structure should be. The result is best defined in the purpose section. The perspective of the project is to investigate the machine learning problem in the domain of racing. The target is not only to solve the racing problem, but also to discuss aspects of machine learning itself.

### Theory
Up to this point the focus was to get a general understanding of the field and an initial judgement on what would be a good approach. During the writing process, we found that we needed to return to the literature for details. Current judgement
 - Supervised learning, approach 1: Train a network to output specific output. Not primarily interesting since correct output is needed in the training process. In a sense, this approach means that the AI is trained to follow a predefined behaviour. This require some other method to get what the desired behaviour should be. Possibly useful in the initial training of the network, to train some basic use cases, but probably only if other methods fail.
 - Supervised learning, approach 2: Train a network to classify a corner type, then to map it to some behaviour, possibly a different network.
 - Unsupervised learning: Possibly useful in a similar way as mentioned for "Supervised learning, approach 2". Since unsupervised learning is used only(?) for classification, other than similar use is probably not useful. 
 - Reinforcement learning: Feedback seems, in the current modelling of the problem, like a good approach generally. The traditional context of markov chains are not relevant though
 - Evolutionary methods. A subset of reinforcement learning. The simulation can be used to score an AI. Changes to network done stochastically.
    - Change weights
    - Change topology
    - Manage species to not kill premature networks to early.

# Week 3, February 1-7 (updated 15 febr)
No supervision meeting. We did not think that we had made enough progress since last week.

We decided to be more thorough in our document. This include discussion content, argument, progress, current tasks and problems. We will evaluate the level of ambition at a later time.

Daniel read some on reinforcement learning.

### Planning report
Started to stake down the contents of the report and started producing text.

### Implementation
Mayor part of the week went to work on the implementation.

Firstly, Martin and Simon made a representation of a track and car and made a simple graphical representation.

Gabriel and Daniel worked on some simple formula's for the physic simulation of the car. Gabriel got for a while stuck on the implementation since it was the first time he used c++ and the specific vector library. The implemented features was:
 - Limited traction
 - Increasing down force for higher speeds
 - Worsened capability of steering while accelerating/braking
The formulas used was 
 - F_max(v) = force on tyres * grip constant = (Downforce(v) + Weight)*µ = (dv^2 + mg)µ
 - Wikipedia dv^2=mg at 130km/h => d=mg/v^2=4.82
 - Assuming same maximum force on all directions of the tyre, accelerating/braking limit steering: F_c = sqrt(F_max^2 - F_acc^2)
 - F_c = mv^2/r (accelerating force of circular motion)
 - => r(v) = mv^2/(F_c(v))
 - How much should float car rotate after dt time, assuming only steering as much the grip holds: dAngle = v * dt / r(v)
 
# Week 2, January 25-31
During the initial supervision meeting with Prasad. Among other things he suggested us to get started with a really simple problem and get a simple simulation up and running fast.

That made us come up with a sequence of rough milestones to pursue. 

1. Make the car drive forward
2. Be able to follow a curve
3. Be able to drive though a sequence of curves
4. Optimise time spent though a single curve by ...

 - ... adapting the speed.
 - ... adapting the car's positioning.

5. Optimise time spent though a sequence of curves by ...

 - ... adapting the speed.
 - ... adapting the car's positioning.

6. Apply more advanced physics. (may be done in parallel with 4 and 5)

 - Drag
 - Down force
 - Tyre wear
 - Fuel consumption

### Research

Otherwise, we did not make much progress on the literature studies. Gabriel found two papers about the same problem but that used modified A* algorithms ( not machine learning), "An AI tool: Generating paths for racing game" and "Game AI: Simulating Car Racing Game by Applying Pathfinding Algorithms".


### Programming language
It was decided to use C++. Arguments for:

- It exists some neural network libraries. It is though not investigated and decided if they will be used. 
- The group is comfortable with it, even if not everyone are experienced.
- It works well enough cross platform
- Ability to optimize the code if desired
- Easy to get started with simple graphics and gui. Also possible to improve graphics if desired
- A good possibility to learn and get experience

The second best option was Java. Although slightly easier and more experience, it is slightly less flexible in making improvements as C++.

Martin and Simon started to set up the development environment.

# Week 1, January 18-24
Set up:

- A web page with all administrative information as requested by supervisor Prasad. http://pendla.github.io/formula-ml/ 
- Git repo. See link on web page

Started literature studies, but nobody have yet had a particular amount of time for it. We have a few relevant sources for machine learning, racing theory and physics. 

We have discussed the problem domain and pinpointed some possibilities and difficulties but we need to investigate possible strategies and techniques much more.

Wrote an initial summary in the report of the progress so far.
