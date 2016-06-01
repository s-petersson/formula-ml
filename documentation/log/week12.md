# Week 12, April 4 - 10
Less tempo due to Easter holy day

During the week, we wrote the first content and discussed the overall structure of the report.

During the Monday meeting, we had some brain storming/summery of previous ideas in two subjects: Future implementation of new techniques and new experiments to conduct. See meeting 160404.

## Gabriel
### Notes: Writing report introduction
I thought of what kind of project we are doing, what it is that should be introduced. I came up with the following summary of our project so far:

TECHNIQUES
neat
(exhaustive search)

TWEAKING
input data
training task: track & controlling responsibilities (output data)
evaluation, fitness function
simulation settings

IMPLEMENTATION
simulation
gui
design/architecture
save/load

Furthermore I thought on what the purpose of the report should be. It is slightly complicated since we probably will reach the most interesting, and first of all most presentable, results during the next month of work. After some discussion with the rest of the group we came up with the following purpose:
The exploration of NEAT in context of the racing problem. How it may be used and what role it may have.

## Martin

Implemented the XOR experiment for NEAT, to detect issues with our implementation.

## Simon

I had some problems with OpenGL rendering on my Macbook, so I spent time trying to figure out how to get the application to run on my computer again. There was also some bugs that occured due to the concurrent nature of our application. I spent time trying to figure out what was wrong there. We still had some inconsistencies between the visualisation and the training, but this time it was not because of the distance driven algorithm. It turned out to be the training algorithm that was setting a new generation as the best one, at an incorrect place in the process.

Furthermore, I spent some time writing on the results part of our report. We don't have very much content yet. But I started to work on some placeholder text from what we've got, as well as setting up a structure that will benefit us later.