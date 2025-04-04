/**
 * @file DiskFileWriter.h
 * @brief Declares the DiskFileWriter class for writing files to disk.
 *
 * This header file defines the DiskFileWriter class that implements the IFileWriter interface.
 * DiskFileWriter writes header and source files into a <outputFolder> directory,
 * cleaning the "ROOT/" prefix from file paths and creating necessary directories.
 */

#pragma once

#include "IFileWriter.h"

#include <string>

/**
 * @namespace GeneratedFileWriter
 * @brief Provides file writing operations for generated code.
 *
 * This namespace contains classes and functions to facilitate writing generated code files to disk.
 * It includes:
 * - The IFileWriter interface, which defines the contract for writing header and source files.
 * - Concrete implementations, such as DiskFileWriter, that handle tasks such as:
 *   - Removing the "ROOT/" prefix from file paths.
 *   - Creating necessary directories under the <outputFolder> folder.
 *   - Writing header files into <outputFolder>/include/ and source files into <outputFolder>/src/.
 *
 * The implementations within this namespace ensure that file generation is performed efficiently,
 * with immediate write-out to disk to minimize memory usage.
 */
namespace GeneratedFileWriter
{

    /**
     * @brief Implementation of IFileWriter that writes files to disk.
     *
     * DiskFileWriter writes header files to <outputFolder>/include/ and source files to
     * <outputFolder>/src/. It ensures that target directories exist by creating them if needed
     * and cleans file paths by removing the "ROOT/" prefix.
     */
    class DiskFileWriter : public IFileWriter
    {
    public:
        /**
         * @brief Constructs a new DiskFileWriter.
         *
         * @param oF The output folder for generated files. Optional, by default is
         * "generatedOutputs".
         */
        DiskFileWriter(const std::string &oF = "generatedOutputs")
            : outputFolder(oF)
        {
        }

        /**
         * @brief Writes a header file to disk.
         *
         * The file path provided will have the "ROOT/" prefix removed before being written
         * to the <outputFolder>/include/ directory.
         *
         * @param filePath The relative file path for the header file.
         * @param content The content to be written to the header file.
         */
        void writeHeaderFile(const std::string &filePath, const std::string &content) override;

        /**
         * @brief Writes a source file to disk.
         *
         * The file path provided will have the "ROOT/" prefix removed before being written
         * to the <outputFolder>/src/ directory.
         *
         * @param filePath The relative file path for the source file.
         * @param content The content to be written to the source file.
         */
        void writeSourceFile(const std::string &filePath, const std::string &content) override;

    private:
        const std::string outputFolder; //**< Output folder for generated files */
    };

} // namespace GeneratedFileWriter
