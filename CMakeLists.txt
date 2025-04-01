cmake_minimum_required(VERSION 3.16)
project(ScaffolderProject VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Optionally, add the include directory for public headers.
include_directories(${PROJECT_SOURCE_DIR}/include)

# Collect production sources from the subdirectories under src/
file(GLOB_RECURSE PRODUCTION_SOURCES
    ${PROJECT_SOURCE_DIR}/src/generator/*.cpp
    ${PROJECT_SOURCE_DIR}/src/parser/*.cpp
    ${PROJECT_SOURCE_DIR}/src/model/*.cpp
    ${PROJECT_SOURCE_DIR}/src/properties/*.cpp
)

# Create the library target first.
add_library(scaffolder_lib ${PRODUCTION_SOURCES})

# Now set the include directories for scaffolder_lib.
target_include_directories(scaffolder_lib PUBLIC
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/include/generator
    ${PROJECT_SOURCE_DIR}/include/parser
    ${PROJECT_SOURCE_DIR}/include/model
    ${PROJECT_SOURCE_DIR}/include/properties
)

# --- Testing Setup ---
enable_testing()

# Find GoogleTest (assumes it's installed on your system or available in your Docker image).
find_package(GTest REQUIRED)

# Gather all test source files.
file(GLOB_RECURSE TEST_SOURCES ${PROJECT_SOURCE_DIR}/tests/*.cpp)

# Create an executable target for tests.
add_executable(UnitTests ${TEST_SOURCES})

# Link the tests with your production library, gtest_main, and pthread if needed.
target_link_libraries(UnitTests PRIVATE scaffolder_lib GTest::gtest_main Threads::Threads)

# Register the test so that 'ctest' can run it.
add_test(NAME UnitTests COMMAND UnitTests)
