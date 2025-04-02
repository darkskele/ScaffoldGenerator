#include "SpecialMemberFunctionParser.h"
#include "ParserUtilities.h"
#include "PropertiesParser.h"
#include "ScaffoldModels.h"
#include <stdexcept>
#include <string>

namespace SpecialMemberFunctionParser
{
    ScaffoldModels::Constructor parseConstructorProperties(const std::string &constructorIdentifier,
                                                           const std::vector<std::string_view> &propertyLines)
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

        // Iterate through each line inside the constructor block.
        for (auto line : propertyLines)
        {
            // Remove any leading/trailing whitespace.
            line = ParserUtilities::trim(line);

            // Skip lines that are empty or don't start with '|'.
            if (line.empty() || line.front() != '|')
                continue;

            // Remove the '|' prefix and re-trim the line.
            line.remove_prefix(1);
            line = ParserUtilities::trim(line);

            // Attempt to split the line into key/value using '='.
            size_t equalPos = line.find('=');
            if (equalPos == std::string_view::npos)
                continue;  // Skip malformed lines without '='.

            // Extract the key and value from the line.
            std::string_view key = ParserUtilities::trim(line.substr(0, equalPos));
            std::string_view value = ParserUtilities::trim(line.substr(equalPos + 1));

            if (key == "parameters")
            {
                // Parse the comma-separated parameter list using a shared utility.
                parameters = PropertiesParser::parseParameters(value);
            }
            else if (key == "description")
            {
                // Remove quotes if present around the description string.
                if (value.size() >= 2 && value.front() == '"' && value.back() == '"')
                {
                    value.remove_prefix(1);
                    value.remove_suffix(1);
                }
                // Convert string_view to std::string and trim it.
                description = std::string(ParserUtilities::trim(value));
            }
            else
            {
                // Throw an error if an unknown property key is encountered.
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

    ScaffoldModels::Destructor parseDestructorProperties(const std::vector<std::string_view> &propertyLines)
    {
        // Destructors only have an optional description; no parameters or other fields.
        std::string description;

        for (auto line : propertyLines)
        {
            line = ParserUtilities::trim(line);

            // Skip if the line is empty or does not start with '|'.
            if (line.empty() || line.front() != '|')
                continue;

            line.remove_prefix(1);
            line = ParserUtilities::trim(line);

            // Attempt to split into key/value.
            size_t equalPos = line.find('=');
            if (equalPos == std::string_view::npos)
                continue;

            std::string_view key = ParserUtilities::trim(line.substr(0, equalPos));
            std::string_view value = ParserUtilities::trim(line.substr(equalPos + 1));

            if (key == "description")
            {
                // Strip quotes from the description if present.
                if (value.size() >= 2 && value.front() == '"' && value.back() == '"')
                {
                    value.remove_prefix(1);
                    value.remove_suffix(1);
                }

                description = std::string(ParserUtilities::trim(value));
            }
            else
            {
                // Destructors must not include unknown properties.
                throw std::runtime_error("Unrecognized property in destructor block: " + std::string(key));
            }
        }

        // Return the parsed destructor model with its description.
        return ScaffoldModels::Destructor(description);
    }
}
