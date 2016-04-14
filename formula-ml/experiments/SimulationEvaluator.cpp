#include "SimulationEvaluator.h"
#include "NeatTrainer.h"
#include <neural/neat/Network.h>
#include <neural/neat/Constants.h>
#include <neural/Helpers.h>

using namespace neat;

SimulationEvaluator::SimulationEvaluator() {

}

SimulationEvaluator::~SimulationEvaluator() {
    if (network_indata.values) delete[] network_indata.values;
    if (network_output.values) delete[] network_output.values;
    if (simulator) {
        if (simulator->track) delete simulator->track;
        if (simulator->car) delete simulator->car;
        //delete simulator;
    }
}

void SimulationEvaluator::init() {
    simulator = new Simulator();

    // Create simulated objects
    // NOTE: Starting grid is at first "checkpoint". In order
    //       to change this, offset the checkpoint order.
    simulator->track = new TrackModel(glm::vec3());
    simulator->car = new CarModel(simulator->track->get_start_grid_pos(), glm::vec3(0, 1, 0), car_speed);

    simulator->progress_timeout = 1.0f;
    simulator->termination_distance = termination_distance;

    if (!network) {
        network = new Network();
    }

    network_indata.value_count = Config::Inputs;
    network_indata.values = new float[Config::Inputs];
    network_output.value_count = Config::Outputs;
    network_output.values = new float[Config::Outputs];

    simulator->carUpdater = [&]() {
        float* inputs = network_indata.values;
        float* outputs = network_output.values;

        int i = 0;
        inputs[i++] = simulator->distance_to_middle();
        inputs[i++] = simulator->angle_to_line();
        inputs[i++] = simulator->car->getSpeed();
        int curve_data_start = i;
        simulator->write_track_curve(inputs, i,
            nbr_of_curve_points,
            curve_point_spacing,
            curve_point_spacing_incremental_percentage);
        inputs[i++] = neural::sum_absolutes(&inputs[curve_data_start],
            simulator->write_track_curve_size(nbr_of_curve_points));
        inputs[i++] = 1.0f;
        neural::flip_parity_if(&inputs[curve_data_start], nbr_of_curve_points, inputs[curve_data_start] > 0);

        network->fire(network_indata, network_output);

        CarControl control;
        control.acceleration = 0;
        control.brake = outputs[1];
        control.steer = outputs[0];
        control.steer = inputs[curve_data_start] > 0 ? control.steer : -control.steer;

        return control;
    };
}

float SimulationEvaluator::evaluate_network(neat::Network* network) {
    SimulationResult result = run(network);
    return neural::fitness_distance_time(result, termination_distance, max_time);
}

SimulationResult SimulationEvaluator::run(neat::Network* network) {
    // Memory responsibility???
    //if (this->network) {
    //    delete this->network;
    //}
    this->network = network;
    reset();
    return simulator->run(0.01f);
}

void SimulationEvaluator::reset() {
    network->reset();
    simulator->reset();
}

std::function<SimulationEvaluator*()> SimulationEvaluator::makeFactory() {
    return [source = *this]() {
        SimulationEvaluator* copy = new SimulationEvaluator(source);
        copy->init();
        return copy;
    };
}