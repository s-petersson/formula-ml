# Week 16, May 2 - May 8
A major focus has been on the report but also to fix some details needed for running the experiments a bit more sophisticated.

We have requested to get access to cloud computing.

All of us also spend some time to test the program and to get a feel for how it behaved.


## Supervision with Broberg
Stand in for prasad. One of the main topics was that we really need to focus on reaching presentable results. See "Meeting 160503" for minutes.


## Gabriel
A lot of time went to the report. I wrote an initial version for the racing and simulation sections in theory and method. Wrote a little text for the results of some experiments and planned for the general discussion. It is however difficult to write much since many of the results have not come yet.

I spent some time to smumerise discussion topics that we have been talking about in the group, or that I have thought of by myself. Since we really do not have results yet, it is barely not possible to elaborate on them yet though.


## Simon
Made the code runnable on linux, and prepared for running on the cloud server.

## Daniel
I've spent most time on the report, writing on different parts in the introduction, some coding was done as well.

### Loading a single file
A quick implementation of loading a single genome into the visualisation was needed in order to show the behaviour of the results, and save images to later use in the presentation and the report.

## Martin
Spent most of my time on the report. 

### Code Optimisation
Since we got access to a server to run experiments on and wanted to runt a lot of experiments in order to get results, we wanted to optimise the code. We felt that it ran quite slow and that it had several bottle-necks where the CPU wasn't fully utilised. By running the visual studio profiler on the code as well as localising the cpu bottlenecks, we were able to speed up the training by at least an order of magnitude, probably two. 

The major speed improvements came from a couple of bug fixes where some functions by mistake returned a vector by value instead of via reference or pointer. This led to large data copies each simulation iteration. 

We also had some issues with CPU utilisation. The evaluation of all the neural networks run in parallel. During this process the CPU runs at full capacity. However, in between each generation, when the algorithm uses the evaluation results to create the next generation of genomes, we had long periods of low CPU utilisation. This was not due to NEAT only running on one core, instead it had to do with the saving of genomes. This process takes the current population and saves them to file, which is quite a lot of work. We solved this by running that code on a separate thread, Thus the evaluation of the new generation could start immediately instead of waiting on the backup to finish. After this fix the bottleneck was more or less removed. 

