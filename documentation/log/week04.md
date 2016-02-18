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



### Gabriel Alpsten

### Daniel Eineving

After reading Adaptive Representations for Reinforcement Learning by Shimon Whiteson I have realized that Reinforcement learning could be of great use for us. As we do not have sample data for our AI to learn from, we can use Reinforcement learning and find a good "gratification" algorithm to solve our problem. Though finding a sufficient "grattification" algorithm would both be hard and key to mange to get this to work. 

The algortihm would need to optimize for time to a specific place on the track, but be so smart to suffer in some stages to get a bigger gratification afterwards. According to Whiteson this outcome is possible to achive with Reinforcement learning.


### Martin Nilsson

### Simon Petersson