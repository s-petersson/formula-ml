cmake_minimum_required(VERSION 3.3)
project(formula_ml)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# Include all the packages that we need in order to compile
# the project.
find_package(OpenGL REQUIRED)

link_directories(${PROJECT_SOURCE_DIR}/external/lib)
include_directories("./")
include_directories(${PROJECT_SOURCE_DIR}/external/include)

# Find and set source files.
add_executable(formula_ml
        Main.cpp
        SimulationState.cpp
        sim/Simulator.cpp
        core/Keyboard.cpp
        core/Window.cpp
        core/WindowState.h
        core/gfx/Program.cpp
        core/util/Camera.cpp
        core/util/Util.cpp
        core/util/Random.cpp
        core/gfx/Model.cpp
        sim/view/CarView.cpp
        sim/model/CarModel.cpp
        sim/model/TrackModel.cpp
        sim/view/TrackView.cpp
        sim/view/GridView.cpp
        neural/FixedNetwork.cpp
        neural/Neural.h
        neural/EvolvingNetwork.cpp
        neural/FixedNetworkTrainer.cpp
        neural/FixedNetworkXOR.cpp
        NeatXOR.cpp
        )

target_link_libraries (formula_ml ${OPENGL_LIBRARIES} ${GLFW_LIBRARIES} ${GLEW_LIBRARY})