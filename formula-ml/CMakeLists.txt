cmake_minimum_required(VERSION 3.2)
project(formula_ml)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O0 -g -pthread")

option(CLOUD_COMPUTING "Compute on the cloud, this removes the OpenGL dependency" ON)

SET(SRCS core/util/Random.cpp
         core/util/Util.cpp
         core/gfx/Model.cpp

         experiments/NeatCurveDataExperiment/NeatCurveDataExperiment.cpp
         experiments/xor/XORExperiment.cpp
         experiments/MultipleTrackExperiment/MultipleTrackExperiment.cpp
         experiments/AlternatingTrackExperiment/AlternatingTrackExperiment.cpp
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

         Main.cpp)

if (CLOUD_COMPUTING)
    add_definitions(-DCLOUD_COMPUTING)
else()
    # Include all the packages that we need in order to compile
    # the project.
    find_package(OpenGL REQUIRED)
    SET(SRCS
            ${SRCS}
            core/gfx/Curve.cpp
            core/gfx/Framebuffer.cpp
            core/gfx/Program.cpp
            core/gfx/Renderer.h
            core/gfx/Texture.cpp
            core/gui/Font.cpp
            core/gui/View.cpp
            core/util/Camera.cpp
            core/util/GLUtil.cpp
            core/util/ImageIO.cpp
            core/Keyboard.cpp
            core/Window.cpp
            core/WindowState.h

            experiments/ManualControl/ManualControl.cpp
            experiments/ExperimentWindow.cpp
            experiments/NetworkView.cpp
            experiments/RacelineLogger.cpp
            experiments/RacelineRenderer.cpp
            experiments/StandardRenderer.cpp

            sim/view/CarView.cpp
            sim/view/GridView.cpp
            sim/view/TrackView.cpp
            sim/SimulationState.cpp
            )
endif ()

include_directories("./")
include_directories(${PROJECT_SOURCE_DIR}/external/include)
link_directories(${PROJECT_SOURCE_DIR}/external/lib)

# Find and set source files.
add_executable(formula_ml ${SRCS})

if (NOT CLOUD_COMPUTING)
    target_link_libraries (formula_ml ${OPENGL_LIBRARIES} glfw3 GLEW)
endif ()