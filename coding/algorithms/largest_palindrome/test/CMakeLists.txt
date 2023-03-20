set(TARGET_NAME ${PROJECT_NAME}_test)

file(GLOB TEST_SOURCES *.cpp)

add_executable(
    ${TARGET_NAME} 
    ${TEST_SOURCES}
    # Files to be tested
    #${SOURCES_DIR}/todo.cpp
)

include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        v1.13.0
)
FetchContent_MakeAvailable(googletest)

target_include_directories(
    ${TARGET_NAME}
    PUBLIC
    SYSTEM
)

target_include_directories(
    ${TARGET_NAME}
    PUBLIC
    .
    ${INC_DIR}
)

target_compile_options(${TARGET_NAME} PUBLIC -Wall -Wextra -pedantic)
target_compile_features(${TARGET_NAME} PUBLIC cxx_std_20)

target_link_libraries(
    ${TARGET_NAME}
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(${TARGET_NAME})
