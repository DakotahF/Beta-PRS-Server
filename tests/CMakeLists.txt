cmake_minimum_required(VERSION 3.8)
set(CMAKE_CXX_STANDARD 11)

add_executable(testprs test-prs-tool.cpp)

file(COPY Data DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

file(COPY test_01 DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
add_test(NAME PRS_PRS COMMAND ${CMAKE_COMMAND} 
        -DPRSCALC=${PROJECT_SOURCE_DIR}/src/prs-calc
        -DTESTPRS=${CMAKE_CURRENT_BINARY_DIR}/testprs
        -P ${CMAKE_CURRENT_SOURCE_DIR}/test_01/prs_compare.cmake)
