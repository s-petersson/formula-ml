#ifndef SIM_SIMULATOR
#define SIM_SIMULATOR

#include <sim/model/CarModel.h>
#include <sim/model/TrackModel.h>

#include <neural/Neural.h>
#include <functional>

struct SimulationResult {
	float distance_driven;
	float time_alive;
};

class Simulator {
public:
	Simulator();
	~Simulator();

	void reset();
	void update(float dt);
	SimulationResult run(float dt);

	// To be set by the user before start:
	CarModel * car;
	TrackModel * track;
	float progress_timeout = 0;
    float termination_distance = std::numeric_limits<float>::max();
    std::function<CarControl()> carUpdater;

	// Readable by user:
	SimulationResult result;

    float distance_to_middle();
    float distance_to_left_edge();
    float distance_to_right_edge();
    float angle_to_line();

    void write_track_curve(float* target, int& offset, int nbr_of_points, float initial_spacing, float spacing_incrementation_percentage);
    static int write_track_curve_size(int nbr_of_points) { return nbr_of_points; };

    void write_checkpoints(float* target, int& offset, int nbr_of_checkpoints);
    static int write_checkpoints_size(int nbr_of_checkpoints) { return 2 * nbr_of_checkpoints; };
	bool has_terminated();
private:
	bool terminated;
	SimulationResult best = SimulationResult();
	float calculate_distance_driven();
};

#endif