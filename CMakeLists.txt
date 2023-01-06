cmake_minimum_required(VERSION 3.17)
project( CARSIMLIB )
# set(CMAKE_CXX_FLAGS "-g") # Debug mode
# set(CMAKE_CXX_FLAGS "-O2 -march=native -std=c++11 -Wall") # release mode 

# set include dirs
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

# build libraries
# add_library(vs_api SHARED ${CMAKE_CURRENT_SOURCE_DIR}/include/vs_get_api.c)
# add_library(carsim_api SHARED ${CMAKE_CURRENT_SOURCE_DIR}/include/CarsimCoreAPI.cpp)
add_library(carsim_lib SHARED ${CMAKE_CURRENT_SOURCE_DIR}/include/CarsimCoreAPI.cpp ${CMAKE_CURRENT_SOURCE_DIR}/include/CarsimManager.cpp)

# link libraries as "carsimlib"
# target_link_libraries(carsim_lib vs_api)

# build sample program
add_executable(samplerun ${CMAKE_CURRENT_SOURCE_DIR}/src/samplerun.cpp)

# for debug
# add_executable(runall ${CMAKE_CURRENT_SOURCE_DIR}/src/runall.cpp)
# target_link_libraries(runall vs_api)

# link carsimlib to the sample program
target_link_libraries(samplerun carsim_lib)
