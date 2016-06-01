# Week 17, May 9 - May 15
Most of the work was denoting to get results from experiments, both on personal machines and from the cloud server, and to write the report.

A lot of work still went to get details in the system to work correctly.
## Daniel
This week had even more focus on the report, as I had to gather data, and interpret the results so taht we could use the training sessions in the report.

### Saving floats
I have caused us a huge bug and loss in training times due to an error where the saving of genomes would not use enough precision when writing the string to the file. On load this would cause the genomes to behave differently and not reach as far in the track as the data showed it would do.

An change had to be made to save the floats as HEX values to properly represent the values with the correct resolution. 

A backlash of this, was that we could no longer interpret the saved genomes and the values of each weigh of the genes in the genome. 

## Gabriel
I ran the the first instances of the the fixed speed experiment. It took some trial and error to find the approximately breaking point for when it was able to finish and not. Wrote about the initial and preleminary results in the report.

After a discussion with Martin, I implemented a different summation of track curvature. Summarising all datapoints, as was implemented before, will provide a very rough value which may be the same for many different scenarios. Now, I added three values that summed different parts of the curvature regions. It does not add more information to the network, but might make it more accessible. It does not need to sum them together itself before using it for other purposes.

I also found the book on racing theory "Fast driving cars", and read that. It is very good to have a solid second source for the racing theory. It did not appear to contradict the previous source, but clarified some aspects.

## Martin

Fixed some issues in the NEAT implementation.
Ran several experiments, mostly for the mirrored track result and results for single corner tracks. 

## Simon
### Experiments
Me and Gabriel prepared a formal list of settings that were to be used during the experiments. So that we have structure in our results, when we are supposed to write about them in our report. When we were done with that,  I ran all the experiments that needed to be run on the cloud computer. This included experiments such as the full control and some fixed speed ones.

### Report
I worked on the report as a whole. We have the deadline for opposition next monday, so we had to spend most of our time writing on the report.


## Experiment configuration
### NEAT configuration
const int Population = 1000;
const float MutateConnectionsChance = 0.70f;
const float PerturbChance = 0.90f;
const float CrossoverChance = 0.75f;
const float LinkMutationChance = 0.50f;
const float NodeMutationChance = 0.40f;
const float BiasMutationChance = 0.40f;
const float StepSize = 0.1f;
const float DisableMutationChance = 0.4f;
const float EnableMutationChance = 0.2f;
const int StaleSpecies = 20;

Sessions 3

Summary fixed speed input configuration:
0	bias
1	middle
2	left
3	right
4	angle
5	10
6	24
7	42
8	66
9	100
10	144
11	205
12	287
13	397
14	546
15	Segment 1, point 1-4, 10-66m
16	Segment 2, point 4-7, 66-205m
17	Segment 3, point 7-10, 205-546m

Summary total control configuration:
0	bias
1	middle
2	left
3	right
4	angle
5	speed
6	10
7	24
8	42
9	66
10	100
11	144
12	205
13	287
14	397
15	546
16	Segment 1, point 1-4, 10-66m
17	Segment 2, point 4-7, 66-205m
18	Segment 3, point 7-10, 205-546m


### Experiment: fixed no curveature
SimulatorSettings sim_settings = SimulatorSettings();
sim_settings.track_path = new string("./res/models/circuit_normal.model");
sim_settings.completeTrack = true;
sim_settings.termination_distance = 5200.f;
sim_settings.max_time = 1000.f;

sim_settings.car_speed_max = 100.f; 
sim_settings.car_speed_initial = 10.f // 15 // 30 // hitta max helt enkelt;
sim_settings.min_avg_speed = 3.f;
sim_settings.avg_speed_excemption_distance = 140.f;


AiSettings ai_settings = AiSettings();
ai_settings.angle_to_line            = true;
ai_settings.distance_to_middle       = true;
ai_settings.distance_to_edges        = true;
ai_settings.speed                    = false;
ai_settings.curve_data               = false;
ai_settings.curve_data_segment_sums  = false;
ai_settings.curve_data_sum_absolutes = false;

ai_settings.output_speed = false;


## Experiment: fixed with curve data
SimulatorSettings sim_settings = SimulatorSettings();
AiSettings ai_settings = AiSettings();

sim_settings.track_path = new string("./res/models/circuit_normal.model");
sim_settings.completeTrack = true;
sim_settings.termination_distance = 5200.f;
sim_settings.max_time = 1000.f;

sim_settings.car_speed_max = 100.f; 
sim_settings.car_speed_initial = 15 // 10 // 25 // hitta max helt enkelt;
sim_settings.min_avg_speed = 3.f;
sim_settings.avg_speed_excemption_distance = 140.f;


ai_settings.angle_to_line            = true;
ai_settings.distance_to_middle       = true;
ai_settings.distance_to_edges        = true;
ai_settings.speed                    = false;
ai_settings.curve_data               = true;
ai_settings.curve_data_segment_sums  = true;
ai_settings.curve_data_sum_absolutes = false;

ai_settings.nbr_of_curve_points = 10;
ai_settings.curve_point_spacing = 10.f;
ai_settings.curve_point_spacing_incremental_percentage = 0.35f;

ai_settings.output_speed = false;


### Experiment Steer and speed control
SimulatorSettings sim_settings = SimulatorSettings();
AiSettings ai_settings = AiSettings();

sim_settings.track_path = new string("./res/models/circuit_normal.model");
sim_settings.completeTrack = true;
sim_settings.termination_distance = 5200.f;
sim_settings.max_time = 1500.f;

sim_settings.car_speed_max = 97.f; 
sim_settings.car_speed_initial = 0.f;
sim_settings.min_avg_speed = 3.f;
sim_settings.avg_speed_excemption_distance = 140.f;


ai_settings.angle_to_line            = true;
ai_settings.distance_to_middle       = true;
ai_settings.distance_to_edges        = true;
ai_settings.speed                    = true;
ai_settings.curve_data               = true;
ai_settings.curve_data_segment_sums  = true;
ai_settings.curve_data_sum_absolutes = false;

ai_settings.nbr_of_curve_points = 10;
ai_settings.curve_point_spacing = 10.f;
ai_settings.curve_point_spacing_incremental_percentage = 0.35f;

ai_settings.output_speed = true;



### Experiment: shortest path
SimulatorSettings sim_settings = SimulatorSettings();
AiSettings ai_settings = AiSettings();

sim_settings.track_path = new string("./res/models/circuit_normal.model");
sim_settings.completeTrack = true;
sim_settings.termination_distance = 5200.f;
sim_settings.max_time = 1000.f;

sim_settings.car_speed_max = 100.f; 
sim_settings.car_speed_initial = 15.f;
sim_settings.min_avg_speed = 3.f;
sim_settings.avg_speed_excemption_distance = 140.f;


ai_settings.angle_to_line            = true;
ai_settings.distance_to_middle       = true;
ai_settings.distance_to_edges        = true;
ai_settings.speed                    = false;
ai_settings.curve_data               = true;
ai_settings.curve_data_segment_sums  = true;
ai_settings.curve_data_sum_absolutes = false;

ai_settings.nbr_of_curve_points = 10;
ai_settings.curve_point_spacing = 10.f;
ai_settings.curve_point_spacing_incremental_percentage = 0.35f;

ai_settings.output_speed = false;

// IMPORTANT: change CarModel.cpp::minTurningRadius() return statement to "return minTurningRadius;"
// ALSO: change CarModel.cpp::smoothChange() "const float max_change_time = 0.01f;"

### Experiment: 90 right
population 500

sim_settings.track_path = new string("./res/models/corners/corner_90_10_r.model");
sim_settings.completeTrack = false;
sim_settings.termination_distance = 925.f;
sim_settings.max_time = 400.f;

sim_settings.car_speed_max = 97.f; 
sim_settings.car_speed_initial = 0.f;
sim_settings.min_avg_speed = 3.f;
sim_settings.avg_speed_excemption_distance = 140.f;


ai_settings.angle_to_line            = true;
ai_settings.distance_to_middle       = true;
ai_settings.distance_to_edges        = true;
ai_settings.speed                    = true;
ai_settings.curve_data               = true;
ai_settings.curve_data_segment_sums  = true;
ai_settings.curve_data_sum_absolutes = false;

ai_settings.nbr_of_curve_points = 10;
ai_settings.curve_point_spacing = 10.f;
ai_settings.curve_point_spacing_incremental_percentage = 0.35f;

ai_settings.output_speed = true;



### Experiment: 180 right
population 500

sim_settings.track_path = new string("./res/models/corners/corner_180_r_new.model");
sim_settings.completeTrack = false;
sim_settings.termination_distance = 975.f;
sim_settings.max_time = 400.f;

sim_settings.car_speed_max = 97.f; 
sim_settings.car_speed_initial = 0.f;
sim_settings.min_avg_speed = 3.f;
sim_settings.avg_speed_excemption_distance = 140.f;


ai_settings.angle_to_line            = true;
ai_settings.distance_to_middle       = true;
ai_settings.distance_to_edges        = true;
ai_settings.speed                    = true;
ai_settings.curve_data               = true;
ai_settings.curve_data_segment_sums  = true;
ai_settings.curve_data_sum_absolutes = false;

ai_settings.nbr_of_curve_points = 10;
ai_settings.curve_point_spacing = 10.f;
ai_settings.curve_point_spacing_incremental_percentage = 0.35f;

ai_settings.output_speed = true;

### Experiment: 30 right
population 500

sim_settings.track_path = new string("./res/models/corners/corner_30_r.model");
sim_settings.completeTrack = false;
sim_settings.termination_distance = 820.f;
sim_settings.max_time = 150.f;

sim_settings.car_speed_max = 97.f; 
sim_settings.car_speed_initial = 0.f;
sim_settings.min_avg_speed = 3.f;
sim_settings.avg_speed_excemption_distance = 140.f;


ai_settings.angle_to_line            = true;
ai_settings.distance_to_middle       = true;
ai_settings.distance_to_edges        = true;
ai_settings.speed                    = true;
ai_settings.curve_data               = true;
ai_settings.curve_data_segment_sums  = true;
ai_settings.curve_data_sum_absolutes = false;

ai_settings.nbr_of_curve_points = 10;
ai_settings.curve_point_spacing = 10.f;
ai_settings.curve_point_spacing_incremental_percentage = 0.35f;

ai_settings.output_speed = true;