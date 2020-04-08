cmake_minimum_required(VERSION 3.8)
set(CMAKE_CXX_STANDARD 11)
set(CGET_PREFIX /net/hunt/home/kotah/prs-server-beta/PRS-methods/prs-toolchain/cget)
set(CMAKE_BUILD_TYPE Debug)
find_library(SAVVY_LIB savvy HINT ${CGET_PREFIX}/lib)
find_library(HTS_LIB hts HINT ${CGET_PREFIX}/lib)
find_library(Z_LIB NAMES libz.a z HINT ${CGET_PREFIX}/lib)
find_package(Threads REQUIRED)

if(${CGET_PREFIX}/lib)
    include_directories(${CGET_PREFIX}/include)
endif()

#set(SOURCE_FILES main.cpp  Weights.cpp Weights.h Individuals.cpp Individuals.h Genotypes.cpp Genotypes.h)
set(SOURCE_FILES main-with-gen.cpp Weights.cpp Weights.h Individuals.cpp Individuals.h)

#add_executable(main ${SOURCE_FILES})
add_executable(main-with-gen ${SOURCE_FILES})
#target_link_libraries(main ${SAVVY_LIB} ${HTS_LIB} ${Z_LIB} Threads::Threads)
target_link_libraries(main-with-gen ${SAVVY_LIB} ${HTS_LIB} ${Z_LIB} Threads::Threads)

#install(TARGETS main DESTINATION ${PROJECT_SOURCE_DIR}/)
install(TARGETS main-with-gen DESTINATION ${PROJECT_SOURCE_DIR}/)

