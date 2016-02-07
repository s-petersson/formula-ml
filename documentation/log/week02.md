## Week 2, January 25-31
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
