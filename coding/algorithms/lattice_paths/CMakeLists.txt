cmake_minimum_required(VERSION 3.4)

project(lattice_paths)

set(TARGET_NAME ${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 20)

find_package(benchmark REQUIRED)

# Location of header and source files in project
set(INC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(SOURCES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

file(GLOB SOURCES ${SOURCES_DIR}/*.cpp)

add_executable(${TARGET_NAME} ${SOURCES})

target_include_directories(
    ${TARGET_NAME}
    PUBLIC
    ${INC_DIR}
)

target_compile_options(${TARGET_NAME} PUBLIC -Wall -Wextra -pedantic -O0)
target_compile_features(${TARGET_NAME} PUBLIC cxx_std_20)
target_link_libraries(${TARGET_NAME} benchmark::benchmark)
