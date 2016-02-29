#include "NEATNetworkMidline.h"

NEATNetworkMidline::NEATNetworkMidline() {
    trainer = new NeatTrainer();
}

NEATNetworkMidline::~NEATNetworkMidline() {
    delete trainer;
}

void NEATNetworkMidline::run() {
    trainer->run();
}

std::function<CarControl()> NEATNetworkMidline::updater() {
	return [&]() {
		CarControl control;

		return control;
	};
}