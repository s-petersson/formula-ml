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
        core/gfx/Model.cpp
        core/gfx/Model.h sim/view/CarView.cpp sim/view/CarView.h sim/model/CarModel.cpp sim/model/CarModel.h sim/model/TrackModel.cpp sim/model/TrackModel.h sim/view/TrackView.cpp sim/view/TrackView.h sim/view/GridView.cpp sim/view/GridView.h)

target_link_libraries (formula_ml ${OPENGL_LIBRARIES} ${GLFW_LIBRARIES} ${GLEW_LIBRARY})