#pragma once

/**
 * @file listFiles.hpp
 * @brief Provides file listing and directory inspection utilities.
 *
 * This header defines an inline function listFilesInDirectory() which
 * displays files from a directory in a formatted, type-grouped output
 * with color coding by file extension. Files are grouped by type
 * (Images, Videos, Audio, etc.) using the mappings from getFileTypes().
 */

#include <iostream>
#include <iomanip>
#include <map>
#include <vector>
#include <algorithm>
#include <filesystem>
#include <limits>

#include "colors.hpp"       ///< ANSI color code macros
#include "header.hpp"       ///< Header display utilities
#include "fileTypes.hpp"    ///< File type mappings
#include "getColor.hpp"     ///< Color selection by file extension

namespace fs = std::filesystem;


/**
 * @brief Display all files in a directory, grouped by file type.
 *
 * Lists all regular files in the given directory, organizes them by type
 * (using the mapping from getFileTypes()), and prints them with:
 * - Color-coded filenames (via getColorForExtension())
 * - File extensions displayed in parentheses
 * - Files grouped under type headers (Images, Videos, Audio, etc.)
 * - A total file count at the end
 *
 * If the directory does not exist or is empty, appropriate messages are displayed.
 * The function pauses and waits for user input (Enter) before returning,
 * making it suitable for interactive TUI use.
 *
 * @param directoryPath The filesystem path to the directory to list.
 *
 * @note The function uses inline implementation; it can be included in
 *       multiple translation units without linker issues.
 *
 * @see getFileTypes() for the typeextension mappings
 * @see getColorForExtension() for color selection by file extension
 */
inline void listFilesInDirectory(const fs::path &directoryPath)
{
    Header::display();
    
    // Validate input directory
    if (!fs::exists(directoryPath))
    {
        std::cout << BOLD << RED << "Directory does not exist.\n" << RESET;
        return;
    }
    if (!fs::is_directory(directoryPath))
    {
        std::cout << RED << "The provided path is not a directory.\n" << RESET;
        return;
    }

    // Display directory header
    std::cout << BOLD << YELLOW << "Listing Files in: "
              << GREEN << directoryPath << RESET << "\n";
    std::cout << DIM << "------------------------------------------------------------------\n"
              << RESET;

    // Build extension  type lookup table (case-insensitive)
    std::map<std::string, std::string> extToType;
    for (const auto &[type, exts] : getFileTypes())
    {
        for (const auto &e : exts)
        {
            std::string lowerExt = e;
            std::transform(lowerExt.begin(), lowerExt.end(), lowerExt.begin(),
                           [](unsigned char c){ return std::tolower(c); });
            extToType[lowerExt] = type;
        }
    }

    // Define the order in which file types should be displayed
    std::vector<std::string> typeOrder =
        {"Images", "Videos", "Audio", "Documents", "Archives", "Code", "Other"};

    // Group files by type for organized display
    std::map<std::string, std::vector<fs::path>> groupedFiles;
    size_t fileCount = 0;// Total file counter

    // Iterate over directory entries and categorize each file
    for (const auto &entry : fs::directory_iterator(directoryPath))
    {
        if (!entry.is_regular_file())
            continue;

        std::string ext = entry.path().extension().string();
        std::string lowerExt = ext;
        std::transform(lowerExt.begin(), lowerExt.end(), lowerExt.begin(),
                       [](unsigned char c){ return std::tolower(c); });

        // Classify file by extension; default to "Other"
        if (!lowerExt.empty() && extToType.find(lowerExt) != extToType.end())
            groupedFiles[extToType[lowerExt]].push_back(entry.path());
        else
            groupedFiles["Other"].push_back(entry.path());

        ++fileCount;
    }

    // Display results
    if (fileCount == 0)
    {
        std::cout << BOLD << RED << "No files found in this directory.\n" << RESET;
    }
    else
    {
        // Display files grouped by type in predefined order
        for (const auto &type : typeOrder)
        {
            auto it = groupedFiles.find(type);
            if (it == groupedFiles.end() || it->second.empty())
                continue;

            // Print type header
            std::cout << BOLD << WHITE << "-- " << type << " --" << RESET << "\n";
            
            // Print each file with color coding and extension
            for (const auto &p : it->second)
            {
                std::string ext = p.extension().string();
                std::string color = getColorForExtension(ext);
                std::cout << color
                          << std::left << std::setw(60) << p.filename().string()
                          << RESET << DIM << " (" << ext << ")\n" << RESET;
            }
            std::cout << "\n";
        }

        // Display footer with total count
        std::cout << DIM << "------------------------------------------------------------------\n" << RESET;
        std::cout << GREEN << "Total files: " << WHITE << fileCount << RESET << "\n\n";
    }

    // Pause for user acknowledgment
    std::cout << YELLOW << "Press Enter to return to the menu..." << RESET;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}
