# Copyright (c) 2022, Piotr Staniszewski

cmake_minimum_required(VERSION 3.0)
project(huffman_coding)

set(TARGET_NAME huffman_coding)

set(CMAKE_CXX_STANDARD 17)

# Project directory
set(PROJECT_DIR ${CMAKE_CURRENT_SOURCE_DIR})

# Location of header and source files in project
set(PROJECT_INC_DIR ${PROJECT_DIR}/inc)
set(PROJECT_SRC_DIR ${PROJECT_DIR}/src)

file(GLOB SOURCES ${PROJECT_SRC_DIR}/*.cpp)

# Version of project and build timestamp
set(VERSION_MAJOR 0)
set(VERSION_MINOR 1)
string(TIMESTAMP BUILD_TIMESTAMP "%Y-%m-%d %H:%M")

configure_file(
    ${PROJECT_INC_DIR}/version.h.in
    ${PROJECT_BINARY_DIR}/version.h
)

add_executable(${TARGET_NAME} ${SOURCES})

target_include_directories(
    ${TARGET_NAME}
    PUBLIC
    ${PROJECT_BINARY_DIR}
    ${PROJECT_INC_DIR}
)

target_compile_options(${TARGET_NAME} PUBLIC -Wall -Wextra -pedantic)
target_compile_features(${TARGET_NAME} PUBLIC cxx_std_17)

enable_testing()
add_subdirectory(test EXCLUDE_FROM_ALL)
