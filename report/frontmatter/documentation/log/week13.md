# Week 13, April 11 - 17
On Tuesday, we had a supervision meeting with Fackspråk concerning the report. The main point of the discussion was the structure.

Gabriel made continued with refactoring experiments and Daniel made some changes to the car controller.


## Report supervision

Handledningstillfälle 2
Tid: 13:15-14:00
Datum: 2016-04-12
Plats: Seminarierum 2
Handledare: Calle Carlsson
E-post: caca@chalmers.se


### Questions
#### Theory Section
Where should background theory be placed?
Within the relevant sections in the Method chapter?
As a standalone theory chapter?
In the problem formulation in the introduction?
As an appendix?

#### Structure choice
Should we have separate Result and Discussion chapters or merge them? We think merged? We have several experiments that we will write about, thus it makes sense to cover both the result and discussion of a single experiment at once.

During the project so far we have focused heavily on a specific machine learning technique called NEAT because other techniques are not as applicable to our specific problem. We are discussing whether or not the report should focus on our solution using NEAT or machine learning techniques in general, comparing and discussing the solution using NEAT to other machine learning techniques and why we choose NEAT. Should we motivate and narrow the focus in the introduction chapter or the method chapter?

The project have a simulator that we have developed, with limited physics. The project will not focus on how to build a simulator, but how much and where should the simulator be mentioned/explained in the report?

Where and how should we argue for that we have built our own simulator and not used an already existing simulator.

#### Reference style
We study at the department of Computer Science and we currently use IEEE. Thoughts?


### Notes
Concerning the introduction of racing theory, strive towards a pure method chapter. Put it somewhere before.

Suggestion on what to call an eventual theory chapter: Theoretical framework.

Concerning the placement of the motivation for neat, is the choice a method in itself or is the algorithm the method? If we test and evaluate different algorithms, it is probably part of the method. It may also be considered as a part of a pre-study, then it is not method. Also, if the different alternatives takes a lot of volume to introduce, it probably fit well in to some kind of theory chapter and definitely not in the introduction. In the view of our target group, it should need to take volume?

We may argue in the "Theory" part for why we use NEAT. 

It is not a problem that that the theory chapter may be very broad, introduce the different parts at the start. Check this with Prasad!

Why have we made a custom made simulator? Sounds like a part of the method. Customisation och control sounds like a valid reasons. Maybe mentions simulation in general in the theory chapter, how we used it and why in the method.

IEEE, yes!

Styckesformattering, radbrott eller indrag!

Ang informella källor, de är ok att använda om de är lämpliga. Racingteorin är kanske inte så formell. Många områden inom IT disciplinen är inte så akademiska.

Write to your "december me". Introduce theory that is relevant but not commonly known in our education.

Förslag: Kan det vara intressant att visa pseudokod i resultat?


## Gabriel
### Suggestion: Program design for experiments
Extract the curve evaluator from NeatCurveDataExperiments. Develop it slightly to let it hold the following responsibilities:
 - Create a simulator with the additional needed resources 
 - Run the simulation, either to return a SimulationResult or a standard fitness value
 - Provide different settings and parameters for the simulation and the ai
 - Ability to change network
It can then be used as it is in NeatTrainer as today, to run one track with the standard fitness function, or as a component for a different type of experiments. An implementation of a experiment may have an own Evaluator that uses the CurveEvaluator. Examples:
 - Call the method in CurveEvaluator that returns a SimulationResult, and use a different fitness function.
 - Have multiple CurveEvaluators that run different things and combine the results to one single fitness value.
 - Have multiple CurveEvaluators and alternate between them. The callback onGenerationDone in NeatTrainer may then be used to update which variant that should be used.
CurveEvaluator may become very general, or we might write different implementations for different purposes. Might save work to have it general. 
In order to activate different features a settings struct with a number of different flags may be used. This may define which resources are allocated and what operations are done in the CarUpdater. A function may derive what number of inputs and outputs are required for the network, to automate the counting and maybe write out which inputs and outputs are which.

### Analysis of curve archetypes
One way to find and distinguish different curve archetype may be to start with features of a optimal behaviour, and then working the way back to how the curve may look. A sophisticated choice of curves in training, may consist of a balanced permutation of different features. Features may be:
- Speed: High, medium or low
- Positioning before the turn: inner, outer
- ...

Examples of single right turns
- Medium speed, outer: 90 degree turn that require slight braking and a positioning far to the left
- Low speed, outer: Hairpin
- High speed, outer: Kink in a straight that do not require the driver to brake at all
- High/medium speed, inner: a slow turning curve, where one can drive the shortest path at maximum speed.

Examples of right-left curves
- Medium speed, outer for both turns: Two rather separated curves that essentially is to single curves
- Medium speed, outer for the first, middle for the second: Two close curves


## Daniel

### Steering
The steering of the car is currently very simplified in the current simulator. By choice we have made the car such that it cannot turn the steering wheel more than it can handle, so that it would never lose the grip with the tyres. This is a nice simplification, but the mappings with steering and network outputs might be weird and causing problems for the learning when it does not need to. 
Currently the network outputs goes from -1 to 1 with maximum steering to each side, but the car interpret this interval to be at the current possible turning interval. This gives the inconsistent that the steering output 0,05 will never give the same car steering in the simulator. 