// test_cmake_generator.cpp
#include <gtest/gtest.h>
#include "BuildToolsGenerator.h"
#include "ProjectMetadata.h"
#include "GeneratorUtilities.h"
#include "testUtility.h"

using namespace GeneratorUtilities;
using namespace ProjectMetadata;

TEST(CMakeGeneratorTest, BasicOutput) {
    using namespace ProjectMetadata;

    // Create dummy metadata for the project-level library ("proj")
    LibraryMetadata projLib("ROOT", "MyProject", true, {"Boost::boost"});
    // Manually set up subdirectories for the project-level (if needed)
    projLib.subDirectories = {"ROOT"}; // In a real scenario, this might be updated during tree building

    // Create dummy metadata for a non-project-level library ("CoreLib")
    LibraryMetadata coreLib("ROOT/CoreLib", "CoreLib", false, {"Eigen3::Eigen"});
    coreLib.subDirectories = {"CoreLib", "CoreLib/Utils", "CoreLib/Utils/Advanced"};

    // Assemble the metadata into a ProjMetadata instance using a map.
    ProjMetadata meta;
    meta.libraries["proj"] = projLib;
    meta.libraries["CoreLib"] = coreLib;

    // Generate the CMake file content.
    std::string cmakeFile = BuildToolGenerator::generateCmakeLists(meta);

    // Now verify that the output contains expected commands.
    // Check for project definition.
    EXPECT_NE(cmakeFile.find("project(MyProject"), std::string::npos);
    // Check for library target generation for CoreLib.
    EXPECT_NE(cmakeFile.find("add_library(CoreLib"), std::string::npos);
    // Check that include directories for CoreLib subdirectories are added.
    EXPECT_NE(cmakeFile.find("target_include_directories(CoreLib PUBLIC ${CMAKE_SOURCE_DIR}/include/CoreLib/"), std::string::npos);
    EXPECT_NE(cmakeFile.find("target_include_directories(CoreLib PUBLIC ${CMAKE_SOURCE_DIR}/include/CoreLib/Utils/"), std::string::npos);
    EXPECT_NE(cmakeFile.find("target_include_directories(CoreLib PUBLIC ${CMAKE_SOURCE_DIR}/include/CoreLib/Utils/Advanced/"), std::string::npos);
    // Check that dependency commands for the project-level target are generated.
    EXPECT_NE(cmakeFile.find("find_package(Boost REQUIRED)"), std::string::npos);
    // Check that the main binary target linking to libraries is present.
    EXPECT_NE(cmakeFile.find("target_link_libraries(${MAIN_TARGET} PUBLIC CoreLib)"), std::string::npos);
}

TEST(CMakeGeneratorTest, MinimalProjectWithOneLibrary) {
    // Create project-level metadata ("proj")
    LibraryMetadata projLib("ROOT", "MyProject", true, {"Boost::boost"});
    // Set up subdirectories for the project-level target.
    projLib.subDirectories = {"ROOT", "ROOT/Include"};
    
    // Create a non-project-level library (CoreLib) with one dependency.
    LibraryMetadata coreLib("ROOT/CoreLib", "CoreLib", false, {"Eigen3::Eigen"});
    // Assume CoreLib has several nested subdirectories.
    coreLib.subDirectories = {"CoreLib", "CoreLib/Utils", "CoreLib/Utils/Advanced"};
    
    // Assemble into ProjMetadata using a map.
    ProjMetadata meta;
    meta.libraries["proj"] = projLib;
    meta.libraries["CoreLib"] = coreLib;
    
    // Generate the CMakeLists.txt content.
    std::string cmakeFile = BuildToolGenerator::generateCmakeLists(meta);
    
    // Check that the project is defined.
    EXPECT_TRUE(contains(cmakeFile, "project(MyProject"));
    // Check that the C++ standard is set to 23.
    EXPECT_TRUE(contains(cmakeFile, "set(CMAKE_CXX_STANDARD 23)"));
    
    // Verify that CoreLib target is generated.
    EXPECT_TRUE(contains(cmakeFile, "add_library(CoreLib"));
    
    // Check that include directories for CoreLib subdirectories are added.
    EXPECT_TRUE(contains(cmakeFile, "target_include_directories(CoreLib PUBLIC ${CMAKE_SOURCE_DIR}/include/CoreLib/)"));
    EXPECT_TRUE(contains(cmakeFile, "target_include_directories(CoreLib PUBLIC ${CMAKE_SOURCE_DIR}/include/CoreLib/Utils/)"));
    EXPECT_TRUE(contains(cmakeFile, "target_include_directories(CoreLib PUBLIC ${CMAKE_SOURCE_DIR}/include/CoreLib/Utils/Advanced/)"));
    
    // Check that dependency commands for CoreLib include find_package for Eigen3.
    EXPECT_TRUE(contains(cmakeFile, "find_package(Eigen3 REQUIRED)"));
    EXPECT_TRUE(contains(cmakeFile, "target_link_libraries(CoreLib PUBLIC Eigen3::Eigen)"));
    
    // Check that project-level dependency commands for MyProject include find_package for Boost.
    EXPECT_TRUE(contains(cmakeFile, "find_package(Boost REQUIRED)"));
    EXPECT_TRUE(contains(cmakeFile, "target_link_libraries(MyProject PUBLIC Boost::boost)") ||
                contains(cmakeFile, "target_link_libraries(${MAIN_TARGET} PUBLIC Boost::boost)"));
    
    // Check that the main binary target is created.
    EXPECT_TRUE(contains(cmakeFile, "add_executable("));
}

TEST(CMakeGeneratorTest, MultipleLibrariesAndDependencies) {
    // Create project-level metadata ("proj")
    LibraryMetadata projLib("ROOT", "MyProject", true, {"Poco::Foundation"});
    projLib.subDirectories = {"ROOT", "ROOT/ProjectInclude"};
    
    // Create two non-project-level libraries.
    LibraryMetadata libA("ROOT/LibA", "LibA", false, {"OpenCV::opencv_core", "OpenCV::opencv_imgproc"});
    libA.subDirectories = {"LibA", "LibA/Subfolder"};
    
    LibraryMetadata libB("ROOT/LibB", "LibB", false, {"Qt5::Core"});
    libB.subDirectories = {"LibB"};
    
    // Assemble metadata.
    ProjMetadata meta;
    meta.libraries["proj"] = projLib;
    meta.libraries["LibA"] = libA;
    meta.libraries["LibB"] = libB;
    
    // Generate CMake content.
    std::string cmakeFile = BuildToolGenerator::generateCmakeLists(meta);
    
    // Project settings check.
    EXPECT_TRUE(contains(cmakeFile, "project(MyProject"));
    EXPECT_TRUE(contains(cmakeFile, "set(CMAKE_CXX_STANDARD 23)"));
    
    // Check that LibA target is generated and has its include directories.
    EXPECT_TRUE(contains(cmakeFile, "add_library(LibA"));
    EXPECT_TRUE(contains(cmakeFile, "target_include_directories(LibA PUBLIC ${CMAKE_SOURCE_DIR}/include/LibA/)"));
    EXPECT_TRUE(contains(cmakeFile, "target_include_directories(LibA PUBLIC ${CMAKE_SOURCE_DIR}/include/LibA/Subfolder/)"));
    
    // Check that LibB target is generated.
    EXPECT_TRUE(contains(cmakeFile, "add_library(LibB"));
    EXPECT_TRUE(contains(cmakeFile, "target_include_directories(LibB PUBLIC ${CMAKE_SOURCE_DIR}/include/LibB/)"));
    
    // Verify dependency commands for LibA.
    EXPECT_TRUE(contains(cmakeFile, "find_package(OpenCV REQUIRED)")); // From "OpenCV::opencv_core" or similar.
    EXPECT_TRUE(contains(cmakeFile, "target_link_libraries(LibA PUBLIC OpenCV::opencv_core)"));
    EXPECT_TRUE(contains(cmakeFile, "target_link_libraries(LibA PUBLIC OpenCV::opencv_imgproc)"));
    
    // Verify dependency commands for LibB.
    EXPECT_TRUE(contains(cmakeFile, "find_package(Qt5 REQUIRED)")); // From "Qt5::Core".
    EXPECT_TRUE(contains(cmakeFile, "target_link_libraries(LibB PUBLIC Qt5::Core)"));
    
    // Verify main binary target links against LibA and LibB.
    EXPECT_TRUE(contains(cmakeFile, "target_link_libraries(${MAIN_TARGET} PUBLIC LibA)"));
    EXPECT_TRUE(contains(cmakeFile, "target_link_libraries(${MAIN_TARGET} PUBLIC LibB)"));
    
    // Check that project-level dependency for MyProject is present.
    EXPECT_TRUE(contains(cmakeFile, "find_package(Poco REQUIRED)"));
    EXPECT_TRUE(contains(cmakeFile, "target_link_libraries(${MAIN_TARGET} PUBLIC Poco::Foundation)"));
}

TEST(CMakeGeneratorTest, LibraryWithoutDependencies) {
    // Create project-level metadata ("proj") with no dependencies.
    LibraryMetadata projLib("ROOT", "MyProject", true, {});
    projLib.subDirectories = {"ROOT"};
    
    // Create a non-project-level library without dependencies.
    LibraryMetadata libNoDep("ROOT/LibNoDep", "LibNoDep", false, {});
    libNoDep.subDirectories = {"LibNoDep"};
    
    ProjMetadata meta;
    meta.libraries["proj"] = projLib;
    meta.libraries["LibNoDep"] = libNoDep;
    
    std::string cmakeFile = BuildToolGenerator::generateCmakeLists(meta);
    
    // Check that the project target exists.
    EXPECT_TRUE(contains(cmakeFile, "project(MyProject"));
    // Check that LibNoDep is generated.
    EXPECT_TRUE(contains(cmakeFile, "add_library(LibNoDep"));
    // Ensure that no dependency commands are generated for LibNoDep.
    EXPECT_FALSE(contains(cmakeFile, "find_package(")); // At least for LibNoDep block.
}