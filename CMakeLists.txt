cmake_minimum_required(VERSION 3.16)
project(ScaffolderProject VERSION 1.0.0)

# Set C++20.
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Define directories.
set(INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
set(TEST_DIR ${PROJECT_SOURCE_DIR}/tests)  # Directory containing testUtility.h

# --- Models Library (Header-Only) ---
add_library(models INTERFACE)
target_include_directories(models INTERFACE ${INCLUDE_DIR}/model)

# --- Generator Library ---
file(GLOB_RECURSE GENERATOR_SOURCES ${PROJECT_SOURCE_DIR}/src/generator/*.cpp)
add_library(generator ${GENERATOR_SOURCES})
target_include_directories(generator PUBLIC 
    ${INCLUDE_DIR}/generator
    ${INCLUDE_DIR}   # For shared headers.
)
target_link_libraries(generator PUBLIC models)

# --- Parser Library ---
file(GLOB_RECURSE PARSER_SOURCES ${PROJECT_SOURCE_DIR}/src/parser/*.cpp)
add_library(parser ${PARSER_SOURCES})
target_include_directories(parser PUBLIC 
    ${INCLUDE_DIR}/parser
    ${INCLUDE_DIR}
)
target_link_libraries(parser PUBLIC models)

# --- Testing Setup ---
enable_testing()
find_package(GTest REQUIRED)

# --- Parser Tests ---
file(GLOB_RECURSE PARSER_TEST_SOURCES ${PROJECT_SOURCE_DIR}/tests/parser/*.cpp)
add_executable(ParserTests ${PARSER_TEST_SOURCES})
target_include_directories(ParserTests PRIVATE 
    ${INCLUDE_DIR}    # For headers in include/
    ${TEST_DIR}       # For testUtility.h
)
target_link_libraries(ParserTests PRIVATE 
    parser
    models
    GTest::gtest_main
    Threads::Threads
)
add_test(NAME ParserTests COMMAND ParserTests)

# --- Generator Tests (using an imported parser target) ---
file(GLOB_RECURSE GENERATOR_TEST_SOURCES ${PROJECT_SOURCE_DIR}/tests/generator/*.cpp)
add_executable(GeneratorTests ${GENERATOR_TEST_SOURCES})
target_include_directories(GeneratorTests PRIVATE 
    ${INCLUDE_DIR}
    ${TEST_DIR}       # For testUtility.h
)

# Create an imported target for the parser library.
# Update PARSER_LIB_PATH below to point to your prebuilt parser library binary.
if(NOT TARGET parser_imported)
    add_library(parser_imported STATIC IMPORTED)
    set(PARSER_LIB_PATH "${PROJECT_SOURCE_DIR}/build-parser-tests/libparser.a")
    set_target_properties(parser_imported PROPERTIES
        IMPORTED_LOCATION "${PARSER_LIB_PATH}"
        INTERFACE_INCLUDE_DIRECTORIES "${PROJECT_SOURCE_DIR}/include/parser"
    )
endif()

target_link_libraries(GeneratorTests PRIVATE 
    generator
    parser_imported
    models
    GTest::gtest_main
    Threads::Threads
)
add_test(NAME GeneratorTests COMMAND GeneratorTests)
