#include <gtest/gtest.h>
#include "parser/ClassParser.h"
#include <string_view>

using namespace ClassParser;
using namespace ClassModels;

TEST(ClassParserTest, ParsesBasicClassWithDescription) {
    std::deque<std::string_view> lines = {
        "| description = \"A test class\"",
        "_"
    };

    ClassModel cls = parseClassBlock("TestClass", lines);

    EXPECT_EQ(cls.name, "TestClass");
    EXPECT_EQ(cls.description, "A test class");
}

TEST(ClassParserTest, ParsesDefaultConstructor) {
    std::deque<std::string_view> lines = {
        "- constructor default:",
        "| description = \"Default constructor\"",
        "_",
        "_"
    };

    ClassModel cls = parseClassBlock("TestClass", lines);
    ASSERT_EQ(cls.constructors.size(), 1);
    EXPECT_EQ(cls.constructors[0].type, ConstructorType::DEFAULT);
    EXPECT_EQ(cls.constructors[0].description, "Default constructor");
}

TEST(ClassParserTest, ParsesCustomConstructorWithParams) {
    std::deque<std::string_view> lines = {
        "- constructor custom:",
        "| parameters = name:string, id:int",
        "| description = \"Custom constructor\"",
        "_",
        "_"
    };

    ClassModel cls = parseClassBlock("TestClass", lines);
    ASSERT_EQ(cls.constructors.size(), 1);
    EXPECT_EQ(cls.constructors[0].type, ConstructorType::CUSTOM);
    ASSERT_EQ(cls.constructors[0].parameters.size(), 2);
    EXPECT_EQ(cls.constructors[0].parameters[0].name, "name");
    EXPECT_EQ(cls.constructors[0].parameters[1].name, "id");
}

TEST(ClassParserTest, ParsesDestructor) {
    std::deque<std::string_view> lines = {
        "- destructor:",
        "| description = \"Cleans up resources\"",
        "_",
        "_"
    };

    ClassModel cls = parseClassBlock("TestClass", lines);
    ASSERT_TRUE(cls.destructor.has_value());
    EXPECT_EQ(cls.destructor->description, "Cleans up resources");
}

TEST(ClassParserTest, ParsesAssignmentOperators) {
    std::deque<std::string_view> lines = {
        "| assignment = copy, move",
        "_"
    };

    ClassModel cls = parseClassBlock("TestClass", lines);
    EXPECT_TRUE(cls.hasCopyAssignment);
    EXPECT_TRUE(cls.hasMoveAssignment);
}

TEST(ClassParserTest, ParsesMemberAccessGroups) {
    std::deque<std::string_view> lines = {
        "- public:",
        "| members = id:int, name:string",
        "- method setName:",
        "| return = void",
        "| parameters = name:string",
        "_",
        "_",
        "- private:",
        "| members = data:float",
        "_",
        "_"
    };

    ClassModel cls = parseClassBlock("TestClass", lines);

    ASSERT_EQ(cls.publicMembers.size(), 2);
    ASSERT_EQ(cls.privateMembers.size(), 1);
    ASSERT_EQ(cls.publicMethods.size(), 1);

    EXPECT_EQ(cls.publicMembers[0].name, "id");
    EXPECT_EQ(cls.publicMethods[0].name, "setName");
    EXPECT_EQ(cls.privateMembers[0].name, "data");
}

TEST(ClassParserTest, ParsesEmptyClassBlock) {
    std::deque<std::string_view> lines = { "_" };

    ClassModel cls = parseClassBlock("EmptyClass", lines);

    EXPECT_EQ(cls.name, "EmptyClass");
    EXPECT_EQ(cls.description, "");
    EXPECT_TRUE(cls.constructors.empty());
    EXPECT_FALSE(cls.destructor.has_value());
    EXPECT_TRUE(cls.publicMethods.empty());
    EXPECT_TRUE(cls.privateMethods.empty());
    EXPECT_TRUE(cls.protectedMethods.empty());
}

TEST(ClassParserTest, ThrowsOnInvalidConstructorType) {
    std::deque<std::string_view> lines = {
        "- constructor funky:",  // invalid type
        "| description = \"Invalid type\"",
        "_",
        "_"
    };

    EXPECT_THROW(parseClassBlock("TestClass", lines), std::runtime_error);
}

TEST(ClassParserTest, DefaultConstructorWithParamsThrows) {
    std::deque<std::string_view> lines = {
        "- constructor default:",
        "| parameters = x:int",
        "_",
        "_"
    };

    EXPECT_THROW(parseClassBlock("BrokenClass", lines), std::runtime_error);
}

TEST(ClassParserTest, ThrowsOnUnknownProperty) {
    std::deque<std::string_view> lines = {
        "| nonsense = true",
        "_"
    };

    EXPECT_THROW(parseClassBlock("BadClass", lines), std::runtime_error);
}

TEST(ClassParserTest, DuplicateDestructorThrows) {
    std::deque<std::string_view> lines = {
        "- destructor:",
        "| description = \"One\"",
        "_",
        "- destructor:",
        "| description = \"Two\"",
        "_",
        "_"
    };

    EXPECT_THROW(parseClassBlock("DupDtor", lines), std::runtime_error);
}

TEST(ClassParserTest, IgnoresTrailingGarbage) {
    std::deque<std::string_view> lines = {
        "| description = \"Should parse\"",
        "_",
        "not a block",
        "- unrelated block:"
    };

    ClassModel cls = parseClassBlock("GarbageSafeClass", lines);
    EXPECT_EQ(cls.description, "Should parse");
}

TEST(ClassParserTest, ParsesProtectedSectionCorrectly) {
    std::deque<std::string_view> lines = {
        "- protected:",
        "| members = counter:uint",
        "- method getCount:",
        "| return = uint",
        "_",
        "_"
    };

    ClassModel cls = parseClassBlock("ProtectedClass", lines);
    ASSERT_EQ(cls.protectedMembers.size(), 1);
    ASSERT_EQ(cls.protectedMethods.size(), 1);
    EXPECT_EQ(cls.protectedMembers[0].name, "counter");
    EXPECT_EQ(cls.protectedMethods[0].name, "getCount");
}

TEST(ClassParserTest, MethodsAndMembersDefaultToPrivate) {
    std::deque<std::string_view> lines = {
        "| members = value:int",
        "- method doSomething:",
        "| return = void",
        "| parameters = ",
        "| description = \"A method with default (private) access\"",
        "_",
        "_" 
    };

    ClassModel cls = parseClassBlock("DefaultAccessClass", lines);
    ASSERT_EQ(cls.privateMembers.size(), 1);
    EXPECT_EQ(cls.privateMembers[0].name, "value");
    ASSERT_EQ(cls.privateMethods.size(), 1);
    EXPECT_EQ(cls.privateMethods[0].name, "doSomething");
}

TEST(ClassParserTest, ParsesMultipleMembersAndMethods) {
    std::deque<std::string_view> lines = {
        "- public:",
        "| members = id:int, name:string, age:int",
        "- method getId:",
        "| return = int",
        "| parameters = ",
        "| description = \"Gets ID\"",
        "_",
        "- method getName:",
        "| return = string",
        "| parameters = ",
        "| description = \"Gets name\"",
        "_",
        "- method getAge:",
        "| return = int",
        "| parameters = ",
        "| description = \"Gets age\"",
        "_",
        "_"
    };

    ClassModel cls = parseClassBlock("MultiAccessClass", lines);
    ASSERT_EQ(cls.publicMembers.size(), 3);
    ASSERT_EQ(cls.publicMethods.size(), 3);
}

TEST(ClassParserTest, HandlesExtraWhitespaceAndNewlines) {
    std::deque<std::string_view> lines = {
        "    |    description    =    \"Whitespace test class\"    ",
        "   -   public:   ",
        "    |   members   =   a:int   ,   b:float   ",
        "   -   method   doSomething   :   ",
        "    |   return   =   void   ",
        "    |   parameters   =    ",
        "    |   description   =   \"Method with extra whitespace\"   ",
        "   _   ",
        "   _   ",
        "    _   "
    };

    ClassModel cls = parseClassBlock("WhitespaceClass", lines);
    EXPECT_EQ(cls.description, "Whitespace test class");
    ASSERT_EQ(cls.publicMembers.size(), 2);
    EXPECT_EQ(cls.publicMembers[0].name, "a");
    EXPECT_EQ(cls.publicMembers[1].name, "b");
    ASSERT_EQ(cls.publicMethods.size(), 1);
    EXPECT_EQ(cls.publicMethods[0].name, "doSomething");
}

TEST(ClassParserTest, ThrowsOnInvalidNestedProperty) {
    std::deque<std::string_view> lines = {
        "- method invalidMethod:",
        "| nonsense = 42",  // invalid property key within a method block
        "| return = void",
        "| parameters = ",
        "| description = \"Method with invalid property\"",
        "_",
        "_"
    };

    EXPECT_THROW(parseClassBlock("InvalidNestedProp", lines), std::runtime_error);
}

TEST(ClassParserTest, ParsesInterleavedClassPropertiesAndBlocks) {
    std::deque<std::string_view> lines = {
        "| description = \"Interleaved blocks test\"",
        "- method firstMethod:",
        "| return = int",
        "| parameters = x:int",
        "| description = \"First method\"",
        "_",
        "| assignment = copy",
        "- public:",
        "| members = publicMember:string",
        "- method secondMethod:",
        "| return = string",
        "| parameters = ",
        "| description = \"Second method\"",
        "_",
        "_"
    };

    ClassModel cls = parseClassBlock("InterleavedClass", lines);
    EXPECT_EQ(cls.description, "Interleaved blocks test");
    EXPECT_TRUE(cls.hasCopyAssignment);
    ASSERT_EQ(cls.publicMembers.size(), 1);
    ASSERT_EQ(cls.publicMethods.size(), 1);
    EXPECT_EQ(cls.publicMembers[0].name, "publicMember");
    EXPECT_EQ(cls.publicMethods[0].name, "secondMethod");
    // The first method (firstMethod) is defined before any explicit access specifier,
    // so it should default to private.
    ASSERT_EQ(cls.privateMethods.size(), 1);
    EXPECT_EQ(cls.privateMethods[0].name, "firstMethod");
}

TEST(ClassParserTest, ThrowsOnCompletelyMalformedFile) {
    std::deque<std::string_view> lines = {
        "This is not valid DSL",
        "Just some random text",
        "Another random line",
        ""
    };

    EXPECT_THROW(parseClassBlock("MalformedClass", lines), std::runtime_error);
}
