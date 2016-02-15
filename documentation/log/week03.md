## Week 3, February 1-7 (updated 15 February)
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

### Gabriel Alpsten

### Daniel Eineving

### Martin Nilsson

Wrote in the method and problem definition chapters of the report. Wrote a lot of the basics of the simulator implementations, ie. window handling, rendering etc. 

### Simon Petersson