cmake_minimum_required(VERSION 3.4)

project(largest_palindrome)

set(TARGET_NAME ${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 17)

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

target_compile_options(${TARGET_NAME} PUBLIC -Wall -Wextra -pedantic)
target_compile_features(${TARGET_NAME} PUBLIC cxx_std_17)

enable_testing()
add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND})

add_subdirectory(test EXCLUDE_FROM_ALL)
