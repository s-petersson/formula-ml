#include "Simulator.h"
#include <neural/FixedNetwork.h>
#include <iostream>
#include "glm/ext.hpp"

using namespace neural;

Simulator::Simulator(float avg, float excemption_distance) {
	min_avg_speed = avg;
	avg_speed_excemption_distance = excemption_distance;
	result = SimulationResult();
	terminated = false;
}

Simulator::Simulator() {
	min_avg_speed = 0;
	avg_speed_excemption_distance = 0;
	result = SimulationResult();
	terminated = false;
}

Simulator::~Simulator() {
    delete car;
    delete track;
}

/**
 * Reset all the values for the simulator, this provides flexibility so that new simulators doesn't have to
 * be created when one wants to rerun a simulation. Instead one can simply reset the simulator.
 */
void Simulator::reset() {
    // Reset the simulators values
    result = SimulationResult();

    // Now reset the cars values.
    car->reset();
	terminated = false;
	best = SimulationResult();
}

/**
 * Calculate the angle between two normalized vectors
 */
inline float angle(glm::vec3 v1, glm::vec3 v2) {
    float angle;

    // Set the angle
    float dot = glm::dot(v1, v2);
    if (dot > 0.9999999f) {
        angle = 0;
    }
    else if (dot < -0.9999999f) {
        // TODO: use definition of pi
        // TODO: use a number slightly less than pi, for stability reasons?
        angle = 3.14159265359f; 
    }
    else {
        angle = glm::acos(glm::dot(v1, v2));
    }

    // Set the correct sign
    if (glm::cross(v1, v2).z < 0) {
        angle = -angle;
    }
    return angle;
}
/**
 * Returns the distance to the middle for the current position of the car.
 * TODO Move this to TrackModel and take a vec3 as input, calculate distance to middle from that point.
 */
float Simulator::distance_to_middle() {
    glm::vec3 line = glm::normalize(track->get_checkpoints()[car->checkpoint].middle - track->get_checkpoints()[car->checkpoint - 1].middle);
    glm::vec3 car_pos = car->position - track->get_checkpoints()[car->checkpoint - 1].middle;
    glm::vec3 right = glm::cross(line, glm::vec3(0, 0, 1));
    return glm::dot(car_pos, right);
}

float Simulator::distance_to_left_edge() {
    glm::vec3 line = glm::normalize(track->get_checkpoints()[car->checkpoint].left - track->get_checkpoints()[car->checkpoint - 1].left);
    glm::vec3 carPos = car->position - track->get_checkpoints()[car->checkpoint - 1].left;
    glm::vec3 right = glm::cross(line, glm::vec3(0, 0, 1));
    return glm::abs(glm::dot(carPos, right));
}

float Simulator::distance_to_right_edge() {
    glm::vec3 line = glm::normalize(track->get_checkpoints()[car->checkpoint].right - track->get_checkpoints()[car->checkpoint - 1].right);
    glm::vec3 carPos = car->position - track->get_checkpoints()[car->checkpoint - 1].right;
    glm::vec3 right = glm::cross(line, glm::vec3(0, 0, 1));
    return glm::abs(glm::dot(carPos, right));
}

float Simulator::angle_to_line() {
    glm::vec3 last_checkpoint = track->get_checkpoints()[glm::max(car->checkpoint - 1, 0)].middle;
    glm::vec3 next_checkpoint = track->get_checkpoints()[glm::max(car->checkpoint, 0)].middle;
    glm::vec3 line = glm::normalize(next_checkpoint - last_checkpoint);
    return angle(line, car->direction);
}

/*
Write to 'target' with 'offset'
Updates offset to include what write_track_curve() writes
*/
void Simulator::write_checkpoints(float* target, int& offset, int nbr_of_checkpoints) {

    int current_checkpoint = glm::max(car->checkpoint, 0);

    glm::vec3 next_checkpoint = track->get_checkpoints()[current_checkpoint].middle;
    glm::vec3 last_line = next_checkpoint - car->position;

    // Angle + distance to the next checkpoint, from the perspective of the car
    if (nbr_of_checkpoints >= 1) {
        target[offset++] = angle(car->direction, glm::normalize(last_line));
        target[offset++] = glm::length(last_line);
    }

    // Angle + distance to the next checkpoint, from the perspective of the last checkpoint
    glm::vec3 last_checkpoint = next_checkpoint;
    for (int i = 2; i <= nbr_of_checkpoints; i++) {
        current_checkpoint++;
        next_checkpoint = track->get_checkpoints()[current_checkpoint].middle;

        glm::vec3 next_line = next_checkpoint - last_checkpoint;

        target[offset++] = angle(glm::normalize(next_line), glm::normalize(last_line));
        target[offset++] = glm::length(next_line);

        last_line = next_line;
        last_checkpoint = next_checkpoint;
    }
}

void Simulator::write_track_curve(float* target, int& offset, int nbr_of_points, float initial_spacing, float spacing_incrementation_percentage) {
    float point_spacing = initial_spacing;
    float point_spacing_increment_factor = 1 + spacing_incrementation_percentage;

    // Variables for the mid line
    int checkpoint_index = glm::max(car->checkpoint, 0);
    glm::vec3 next_checkpoint = track->get_checkpoints()[checkpoint_index].middle;
    glm::vec3 last_checkpoint = track->get_checkpoints()[checkpoint_index - 1].middle;

    // The current line segment
    glm::vec3 line = next_checkpoint - last_checkpoint;
    glm::vec3 line_normalized = glm::normalize(line);
    float line_length = glm::length(line);
    float distance_on_line = glm::dot(car->position - last_checkpoint, line_normalized); // Projection of the car

    // Variables to track the traversing position
    float target_distance = point_spacing;
    glm::vec3 next_point = last_checkpoint + line_normalized * distance_on_line;
    glm::vec3 last_point = next_point;
    glm::vec3 last_direction = line_normalized;

    int track_checkpoints_size = (int) track->get_checkpoints().size();

    int i;
    for (i = 0; i < nbr_of_points; i++) {
        bool should_break = false;

        // Skip line segments until target_distance is withing the next line segment
        while (line_length - distance_on_line < target_distance) {
            // Add the distance for the last line segment
            target_distance -= line_length - distance_on_line;
            next_point += line_normalized * (line_length - distance_on_line);
            distance_on_line = 0;

            // If it exist, hop to the next pair of checkpoints
            checkpoint_index++;
            if (checkpoint_index >= track_checkpoints_size) {
                should_break = true;
                break;
            }

            last_checkpoint = next_checkpoint;
            next_checkpoint = track->get_checkpoints()[checkpoint_index].middle;

            // Update current line segment
            line = next_checkpoint - last_checkpoint;
            line_normalized = glm::normalize(line);
            line_length = glm::length(line);
        }
        if (should_break) {
            break;
        }

        // Add the part that remains on the current line segment
        next_point += target_distance * line_normalized;
        distance_on_line += target_distance;

        // Calculate the next direction from the last point
        // Write the angle to the last direction to target
        glm::vec3 new_direction = glm::normalize(next_point - last_point);
        target[offset++] = angle(last_direction, new_direction);
        last_point = next_point;
        last_direction = new_direction;

        // Reset distance counters
        point_spacing *= point_spacing_increment_factor;
        target_distance = point_spacing;
    }
    // If the track ended, write 0
    for (; i < nbr_of_points; i++) {
        target[offset++] = 0;
    }
}

/*
	Run a complete simulation until failure
	Each simulation step update with time dt
*/
SimulationResult Simulator::run(const float dt) {
	while (!terminated) {
		update(dt);
	}
	return result;
}

float Simulator::calculate_distance_on_track() {
    // Save the checkpoints that are currently close to the car.
    Checkpoint last_checkpoint	= track->get_checkpoints()[car->checkpoint - 1];
    Checkpoint checkpoint		= track->get_checkpoints()[car->checkpoint];

    glm::vec3 midline	            = glm::normalize(checkpoint.middle - last_checkpoint.middle);
    float car_scalar                = glm::dot(car->position - last_checkpoint.middle, midline);
    glm::vec3 car_on_midline        = last_checkpoint.middle + car_scalar * midline;
    float distance_to_car           = glm::distance(car_on_midline, last_checkpoint.middle);
    float distance_to_checkpoint    = checkpoint.distance_on_track - last_checkpoint.distance_on_track;

    // Check if car has passed the checkpoint ahead.
    if (distance_to_car >= distance_to_checkpoint) {
        // Increase the checkpoint, since we passed one.
        car->checkpoint++;

        // Redefine the checkpoints, so that they are correct for our calculations above.
        last_checkpoint	= track->get_checkpoints()[car->checkpoint - 1];
        checkpoint		= track->get_checkpoints()[car->checkpoint];

        midline                 = glm::normalize(checkpoint.middle - last_checkpoint.middle);
        car_scalar              = glm::dot(car->position - last_checkpoint.middle, midline);
        car_on_midline          = last_checkpoint.middle + car_scalar * midline;
        distance_to_car         = glm::distance(car_on_midline, last_checkpoint.middle);
        distance_to_checkpoint  = checkpoint.distance_on_track - last_checkpoint.distance_on_track;
    }

    if (car_scalar < 0) {
        return last_checkpoint.distance_on_track;
    } else {
        return last_checkpoint.distance_on_track + distance_to_car;
    }
}

/*
	Update the simulation with one time step dt [seconds]
*/

/*
 * Update the simulation with one time step dt [seconds]
 */
void Simulator::update(float dt) {
    // Now we update the cars driven distance.
    CarControl control = this->carUpdater();
    car->update(dt, control);

    if (result.time_alive > termination_time) {
        terminated = true;
        return;
    }

    // Check if the car is no longer on the track
	if (!track->on_track(car->position)) {
        // Stop the car in that case
        // TODO: Set boolean on car instead, so that it cannot move.
		car->setSpeed(0.0f);
		terminated = true;
		return;
	}


	//Average speeds
	speeds[speed_index] = car->getSpeed();
	if (speed_index == 499) {
		speed_index = 0;
	}
	else {
		speed_index++;
	}
	float sum_speed = 0;
	for (int i = 0; i < 500; i++) {
		sum_speed += speeds[i];
	}
	if (sum_speed / 500 < min_avg_speed && result.distance_on_track > avg_speed_excemption_distance) {
		car->setSpeed(0.0f);
		terminated = true;
		return;
	}

	if (result.distance_on_track >= termination_distance) {
		// Call it quits
		result.distance_on_track = termination_distance;
		terminated = true;
		return;
	}

	// Check for progress
	if (result.distance_on_track > best.distance_on_track) {
		// The car has progressed
		best = result;
	} else if (result.time_alive > best.time_alive + progress_timeout) {
		// No progress for a while
		// Call it quits
		terminated = true;
		return;
	}

    car->distance_on_track = calculate_distance_on_track();

    // Update result
    result.time_alive += dt;
    result.distance_driven += dt * car->getSpeed();
    result.distance_on_track = car->distance_on_track;
}

bool Simulator::has_terminated() {
	return terminated;
}