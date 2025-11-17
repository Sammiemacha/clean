#pragma once
#include <iostream>
#include <filesystem>
#include <limits>
#include <string>
#include "colors.hpp"
#include "header.hpp"

namespace fs = std::filesystem;

/**
 * @file getDirectory.hpp
 * @brief Utilities for prompting and validating a directory path from the user.
 *
 * This header provides a single inline helper `getDirectoryPath()` which
 * interactively prompts the user for a directory path, validates that the
 * path exists and is a directory, and returns the validated absolute path as
 * a `std::string`. Pressing Enter will select the current working directory.
 */

/**
 * @brief Prompt the user for a target directory and validate it.
 *
 * The function displays the application header, prompts the user to enter
 * a directory path (or press Enter to use the current working directory),
 * and validates that the provided path exists and is a directory. If the
 * validation fails the user is prompted again until a valid directory is
 * provided. The returned string is the validated directory path.
 *
 * @return std::string Absolute path to the validated directory.
 *
 * @note The function blocks for user input and is intended for interactive
 *       terminal usage. It returns an empty string only if the standard
 *       input stream is in an error state.
 */
inline std::string getDirectoryPath()
{
    std::string inputPath;
    do
    {
        // Clear any leftover newline and show the header/banner
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        Header::display();

        std::cout << BOLD << WHITE
                  << "\nEnter the full path of the directory you want to clean "
                     "eg (/home/user/Downloads) or (C:\\Users\\User\\Downloads)"
                  << RESET << std::endl;
        std::cout << YELLOW << "\nPress Enter to use the current directory\n>>: " << GREEN;
        std::getline(std::cin, inputPath);
        std::cout << RESET;

        fs::path targetDir;

        if (inputPath.empty())
        {
            targetDir = fs::current_path(); // current working directory
        }
        else
        {
            targetDir = inputPath;
        }

        // Validate if the directory exists and is a directory
        if (!fs::exists(targetDir) || !fs::is_directory(targetDir))
        {
            Header::display();
            std::cerr << RED << "\nError: The specified path \"" << GREEN << inputPath
                      << RED << "\" is invalid or not a directory.\n"
                      << RESET;
            std::cout << YELLOW << "Press Enter to try again..." << RESET;
            inputPath.clear();
            continue;
        }
        else
        {
            inputPath = targetDir.string(); // update inputPath to the validated path
            break;
        }
    } while (inputPath.empty());

    return inputPath;
}
