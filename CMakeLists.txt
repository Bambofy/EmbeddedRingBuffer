cmake_minimum_required(VERSION 3.19)
project(RingBuffer)
enable_testing()

set(CMAKE_CXX_STANDARD 98)

add_executable(RingBuffer main.cpp include/Block.h include/RingBuffer.h)

#############
# Add tests #
#############
FILE(GLOB tests ${PROJECT_SOURCE_DIR}/tests/*)

FOREACH (test ${tests})
    get_filename_component(test_name ${test} NAME)

    message("Adding test: " ${test_name})

    add_executable(${test_name} ${PROJECT_SOURCE_DIR}/tests/${test_name})

    target_include_directories(${test_name} PUBLIC ${PROJECT_SOURCE_DIR}/include)

    add_test(${test_name} ${test_name})
ENDFOREACH ()