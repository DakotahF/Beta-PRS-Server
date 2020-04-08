cmake_minimum_required(VERSION 3.2) 
project(test-app) 
set(CMAKE_CXX_STANDARD 11) 

find_package(savvy REQUIRED)

add_executable(app main.cpp Individuals.cpp Individuals.h Genotypes.cpp Genotypes.h Weights.cpp Weights.h) 
target_link_libraries(app savvy)
