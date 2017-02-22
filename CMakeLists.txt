cmake_minimum_required(VERSION 3.6)
project(ExposureMerger)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SRC_DIR src/com/mnenmenth/exposuremerger)

set(CORE_DIR ${SRC_DIR}/core)

set(CORE
        ${CORE_DIR}/ExposureMerger.cpp
        )

set(SOURCE_FILES ${CORE})
add_executable(ExposureMerger ${SOURCE_FILES})