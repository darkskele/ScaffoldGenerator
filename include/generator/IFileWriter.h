/**
 * @file IFileWriter.h
 * @brief Defines the interface for file writing operations.
 *
 * This header file declares the IFileWriter interface, which specifies methods
 * for writing header and source files to disk. Implementations of this interface
 * will provide the actual file writing functionality.
 */

#pragma once

#include <string>

/**
 * @brief Interface for file writing operations.
 *
 * This abstract class defines methods to write header and source files.
 */
class IFileWriter
{
public:
    /**
     * @brief Virtual destructor.
     */
    virtual ~IFileWriter() = default;

    /**
     * @brief Writes a header file with the specified path and content.
     *
     * @param filePath The relative file path for the header file.
     * @param content The content to be written to the header file.
     */
    virtual void writeHeaderFile(const std::string &filePath, const std::string &content) = 0;

    /**
     * @brief Writes a source file with the specified path and content.
     *
     * @param filePath The relative file path for the source file.
     * @param content The content to be written to the source file.
     */
    virtual void writeSourceFile(const std::string &filePath, const std::string &content) = 0;
};
