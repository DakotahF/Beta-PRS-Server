cmake_minimum_required(VERSION 3.2) 
project(prs-backbone) 
set(CMAKE_CXX_STANDARD 11) 

find_package(savvy REQUIRED) 

add_executable(prs-calc prs-calculator.cpp Individuals.cpp Individuals.h Genotypes.cpp Genotypes.h Weights.cpp Weights.h) 
add_executable(ancestry-calc ancestry/main.cpp ancestry/Ancestry.h ancestry/Ancestry.cpp)
add_executable(prs-runner main.cpp Helpers.cpp Helpers.h Individuals.cpp Individuals.h Genotypes.cpp Genotypes.h Weights.cpp Weights.h ancestry/Ancestry.cpp ancestry/Ancestry.h weight-formatter/Options.h weight-formatter/Options.cpp weight-formatter/Variant.h weight-formatter/Variant.cpp) 
target_link_libraries(prs-calc savvy)
target_link_libraries(prs-runner savvy) 
