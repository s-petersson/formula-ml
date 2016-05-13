# Week 14, May 2 - May 8
A major focus has been on the report but also to fix some details needed for running the experiments a bit more sophisticated.

We have requested to get access to cloud computing.


## Supervision with Broberg
Stand in for prasad. See "Meeting 160503" for minutes.

## Gabriel
A lot of time went to the report. I wrote an initial version for the racing and simulation sections in theory and method. Wrote a little text for the results of some experiments and planned for the general discussion. It is however difficult to write much since many of the results have not come yet.



## Simon
Made the code runnable on linux, and prepared for running on the cloud server.

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

fixed speed input configuration:
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

Total control configuration:
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
sim_settings.track_path = new string("./res/models/circuit_narrow.model");
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

sim_settings.track_path = new string("./res/models/circuit_narrow.model");
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
sim_settings.track_path = new string("./res/models/circuit_narrow.model");
sim_settings.completeTrack = true;
sim_settings.termination_distance = 5200.f;
sim_settings.max_time = 1500.f;

sim_settings.car_speed_max = 97.f; 
sim_settings.car_speed_initial = 0.f; // hitta max
sim_settings.min_avg_speed = 3.f;
sim_settings.avg_speed_excemption_distance = 140.f;


AiSettings ai_settings = AiSettings();
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



### Experiment: kortaste vägen
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

### Experiment: 90 höger
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



### Experiment: 180 höger
population 500

sim_settings.track_path = new string("./res/models/corners/corner_180_r.model");
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




### Experiment: 30 höger
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
