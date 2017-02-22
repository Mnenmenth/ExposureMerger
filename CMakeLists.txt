cmake_minimum_required(VERSION 3.6)
project(ExposureMerger)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SRC_DIR src/com/mnenmenth/exposuremerger)
set(CORE_DIR ${SRC_DIR}/core)
set(IMGP_DIR ${SRC_DIR}/imgprocessing)

set(CORE
        ${CORE_DIR}/ExposureMerger.cpp
        )
set(IMGPROCESSING
        ${IMGP_DIR}/MergeExposures.cpp
        ${IMGP_DIR}/MergeExposures.h
        )

set(SOURCE_FILES
        ${CORE}
        ${IMGPROCESSING}
        )

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

#find_package(Qt5 REQUIRED COMPONENTS Widgets)
find_package(OpenCV REQUIRED)

add_executable(ExposureMerger ${SOURCE_FILES})
target_link_libraries(ExposureMerger ${OpenCV_LIBS})

#qt5_use_modules(ExposureMerger Widgets)
