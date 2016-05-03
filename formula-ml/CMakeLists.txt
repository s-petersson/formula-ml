cmake_minimum_required(VERSION 3.2)
project(formula_ml)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O0")

include_directories("./")
include_directories(${PROJECT_SOURCE_DIR}/external/include)
link_directories(${PROJECT_SOURCE_DIR}/external/lib)

# Find and set source files.
add_executable(formula_ml
        core/util/Random.cpp
        core/util/Util.cpp
        experiments/NeatCurveDataExperiment/NeatCurveDataExperiment.cpp
        experiments/Experiment.h
        experiments/SimulationEvaluator.cpp

        neural/neat/Constants.cpp
        neural/neat/Gene.cpp
        neural/neat/Genome.cpp
        neural/neat/Network.cpp
        neural/neat/Pool.cpp
        neural/neat/Species.cpp
        neural/neat/Trainer.cpp

        neural/EvolvingNetwork.cpp
        neural/FileWriter.cpp
        neural/Helpers.cpp
        neural/Neural.h
        neural/ResultWriter.cpp

#        neural/FixedNetwork.cpp
#        neural/FixedNetworkTrainer.cpp
#        neural/FixedNetworkXOR.cpp

        sim/model/CarModel.cpp
        sim/model/TrackModel.cpp
        sim/Simulator.cpp

        Main.cpp
        )