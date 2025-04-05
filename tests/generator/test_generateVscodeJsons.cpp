#include <gtest/gtest.h>
#include "BuildToolsGenerator.h" 
#include "testUtility.h"

using namespace BuildToolGenerator;

TEST(VsCodeJsonGeneratorTest, LaunchJsonContainsProjectName) {
    // Generate JSON files for a project called "MyProject".
    auto jsonPair = generateVscodeJSONs("MyProject");
    std::string launchJson = jsonPair.first;

    // Check that the launch.json content contains the project name in various places.
    EXPECT_TRUE(contains(launchJson, "\"name\": \"Debug MyProject\""));
    EXPECT_TRUE(contains(launchJson, "${workspaceFolder}/build-MyProject/MyProject"));
    EXPECT_TRUE(contains(launchJson, "\"cwd\": \"${workspaceFolder}/build-MyProject\""));
    EXPECT_TRUE(contains(launchJson, "\"preLaunchTask\": \"Build and Run MyProject\""));
}

TEST(VsCodeJsonGeneratorTest, TasksJsonContainsBuildTask) {
    // Generate JSON files for a project called "MyProject".
    auto jsonPair = generateVscodeJSONs("MyProject");
    std::string tasksJson = jsonPair.second;

    // Check that the tasks.json content contains the correct task label and command.
    EXPECT_TRUE(contains(tasksJson, "\"label\": \"Build and Run MyProject\""));
    EXPECT_TRUE(contains(tasksJson, "mkdir -p build-MyProject"));
    EXPECT_TRUE(contains(tasksJson, "cmake --build . --target MyProject"));
    EXPECT_TRUE(contains(tasksJson, "\"version\": \"2.0.0\""));
}

TEST(VsCodeJsonGeneratorTest, ValidJsonStructure) {
    // Generate JSON files for a project.
    auto jsonPair = generateVscodeJSONs("MyProject");
    std::string launchJson = jsonPair.first;
    std::string tasksJson = jsonPair.second;

    // Basic sanity checks: ensure that both JSON outputs start with '{' and end with '}'.
    EXPECT_EQ(launchJson.front(), '{');
    EXPECT_EQ(launchJson.back(), '}');
    EXPECT_EQ(tasksJson.front(), '{');
    EXPECT_EQ(tasksJson.back(), '}');
}

