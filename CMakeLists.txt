cmake_minimum_required(VERSION 3.8)
project(PRS-Server VERSION 0.0.1)
include(CTest)

set(LAST_MODIFIED "Apr/21/2020") 

add_compile_options(-Wall -Wpedantic)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_STANDARD 11) 
set(CTEST_OUTPUT_ON_FAILURE TRUE)  

add_subdirectory(src)

enable_testing()
add_subdirectory(tests)     
