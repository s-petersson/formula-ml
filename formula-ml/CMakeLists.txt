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
file(GLOB_RECURSE SOURCE_FILES
        ${PROJECT_SOURCE_DIR}/core/*.cpp
        ${PROJECT_SOURCE_DIR}/core/*.h
        ${PROJECT_SOURCE_DIR}/sim/*.cpp
        ${PROJECT_SOURCE_DIR}/sim/*.h
        Main.cpp
        SimulationState.cpp)
add_executable(formula_ml ${SOURCE_FILES})

target_link_libraries (formula_ml ${OPENGL_LIBRARIES} ${GLFW_LIBRARIES} ${GLEW_LIBRARY})