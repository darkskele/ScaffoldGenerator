#include "SpecialMemberFunctionParser.h"
#include "ParserUtilities.h"
#include "PropertiesParser.h"
#include "ScaffoldModels.h"
#include <stdexcept>
#include <string>

namespace SpecialMemberFunctionParser
{
    ScaffoldModels::Constructor parseConstructorProperties(const std::string &constructorIdentifier,
                                                             std::deque<std::string_view>& propertyLines)
    {
        // Map the string identifier to an enum value representing the constructor type.
        ScaffoldModels::ConstructorType type;
        if (constructorIdentifier == "default")
            type = ScaffoldModels::ConstructorType::DEFAULT;
        else if (constructorIdentifier == "copy")
            type = ScaffoldModels::ConstructorType::COPY;
        else if (constructorIdentifier == "move")
            type = ScaffoldModels::ConstructorType::MOVE;
        else if (constructorIdentifier == "custom")
            type = ScaffoldModels::ConstructorType::CUSTOM;
        else
            throw std::runtime_error("Unknown constructor type: " + constructorIdentifier);

        // Prepare containers for parsed parameters and description.
        std::vector<ScaffoldProperties::Parameter> parameters;
        std::string description;

        // Consume property lines until an end-of-scope marker ("_") is reached.
        while (!propertyLines.empty())
        {
            std::string_view line = ParserUtilities::trim(propertyLines.front());
            propertyLines.pop_front(); // Consume the line

            if (line == "_")
                break; // End of the constructor block.

            // Process only lines that start with the '|' character.
            if (line.empty() || line.front() != '|')
                continue;

            // Remove the '|' prefix and trim the line.
            line.remove_prefix(1);
            line = ParserUtilities::trim(line);

            // Find the '=' separator to split the key and value.
            size_t equalPos = line.find('=');
            if (equalPos == std::string_view::npos)
                continue;  // Skip malformed lines.

            std::string_view key = ParserUtilities::trim(line.substr(0, equalPos));
            std::string_view value = ParserUtilities::trim(line.substr(equalPos + 1));

            if (key == "parameters")
            {
                // Parse the comma-separated parameter list.
                parameters = PropertiesParser::parseParameters(value);
            }
            else if (key == "description")
            {
                // Remove quotes if present.
                if (value.size() >= 2 && value.front() == '"' && value.back() == '"')
                {
                    value.remove_prefix(1);
                    value.remove_suffix(1);
                }
                description = std::string(ParserUtilities::trim(value));
            }
            else
            {
                throw std::runtime_error("Unrecognized property in constructor block: " + std::string(key));
            }
        }

        // For standard (non-custom) constructors, parameters are not allowed.
        if ((type == ScaffoldModels::ConstructorType::DEFAULT ||
             type == ScaffoldModels::ConstructorType::COPY ||
             type == ScaffoldModels::ConstructorType::MOVE) && !parameters.empty())
        {
            throw std::runtime_error("Constructor type '" + constructorIdentifier + "' should not have parameters.");
        }

        // Return a fully constructed Constructor model.
        return ScaffoldModels::Constructor(type, parameters, description);
    }

    ScaffoldModels::Destructor parseDestructorProperties(std::deque<std::string_view>& propertyLines)
    {
        // Destructors only support an optional description.
        std::string description;

        while (!propertyLines.empty())
        {
            std::string_view line = ParserUtilities::trim(propertyLines.front());
            propertyLines.pop_front(); // Consume the line

            if (line == "_")
                break; // End of the destructor block.

            // Process only lines that start with the '|' character.
            if (line.empty() || line.front() != '|')
                continue;

            line.remove_prefix(1);
            line = ParserUtilities::trim(line);

            size_t equalPos = line.find('=');
            if (equalPos == std::string_view::npos)
                continue;

            std::string_view key = ParserUtilities::trim(line.substr(0, equalPos));
            std::string_view value = ParserUtilities::trim(line.substr(equalPos + 1));

            if (key == "description")
            {
                if (value.size() >= 2 && value.front() == '"' && value.back() == '"')
                {
                    value.remove_prefix(1);
                    value.remove_suffix(1);
                }
                description = std::string(ParserUtilities::trim(value));
            }
            else
            {
                throw std::runtime_error("Unrecognized property in destructor block: " + std::string(key));
            }
        }

        return ScaffoldModels::Destructor(description);
    }
}
