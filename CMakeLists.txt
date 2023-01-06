cmake_minimum_required(VERSION 3.17)
project( CARSIMLIB )
# set(CMAKE_CXX_FLAGS "-g") # debug mode
set(CMAKE_CXX_FLAGS "-O2 -march=native -std=c++17 -Wall") # release mode 

# set include dirs
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

# build libraries
add_library(carsim_lib SHARED ${CMAKE_CURRENT_SOURCE_DIR}/src/CarsimCoreAPI.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/CarsimManager.cpp)

# build sample program
add_executable(samplerun ${CMAKE_CURRENT_SOURCE_DIR}/example/samplerun.cpp)

# link carsim_lib to the sample program
target_link_libraries(samplerun carsim_lib)
