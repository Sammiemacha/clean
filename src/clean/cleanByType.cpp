#include "../include/clean/cleanByType.hpp"
#include "../include/header.hpp"
#include "../include/colors.hpp"
#include "../include/getColor.hpp"
#include "../include/fileTypes.hpp" 
#include "../include/dangerousExts.hpp"  
#include "../include/json.hpp"

#include <algorithm>
#include <iostream>
#include <set>

/**
 * @file cleanByType.cpp
 * @brief Implements logic to organize files into subdirectories by file type.
 *
 * The primary function provided by this translation unit is `cleanFilesByType()`
 * which inspects files in a given directory, classifies them using the
 * category→extensions mapping provided by `getFileTypes()`, and moves files
 * into type-named subdirectories. Files with extensions marked as "dangerous"
 * by `getDangerousExts()` are skipped.
 */

/**
 * @brief Move files from `directoryPath` into type-based subdirectories.
 *
 * Scans `directoryPath` for regular files, determines each file's extension,
 * and moves the file into a subdirectory named after its type (for example
 * "Images" or "Documents"). Behavior:
 * - Uses `getFileTypes()` to build an extension→type lookup.
 * - Skips files whose lowercase extension appears in `getDangerousExts()`.
 * - Creates the destination directory if it does not exist.
 * - Skips files that would collide with an existing filename in the
 *   destination directory.
 * - Reports counts of moved and skipped files and prints a list of skipped filenames.
 *
 * @param directoryPath Filesystem path to the target directory to organize.
 *
 * @note The function prints prompts and waits for the user to press Enter
 *       before returning so it is suitable for interactive use in a TUI.
 *
 * @warning Uses `std::filesystem::rename()` which may throw or set error codes
 *          depending on platform; errors are reported to `std::cerr` and the
 *          offending file is skipped.
 */
void cleanFilesByType(const fs::path &directoryPath)
{
    Header::display();
    // Counters for reporting
    size_t moved = 0;
    size_t skipped = 0;
    std::vector<fs::path> skippedFiles;

    // Validate input directory
    if (!fs::exists(directoryPath) || !fs::is_directory(directoryPath))
    {
        std::cerr << RED << "Invalid directory provided.\n" << RESET;
        return;
    }

    // Load dangerous extensions dynamically (lowercase, including dot)
    auto dangerous = getDangerousExts();

    // Build extension → type lookup using the file types mapping
    std::map<std::string, std::string> extToType;
    for (const auto &[type, exts] : getFileTypes())
    {
        for (const auto &e : exts)
        {
            std::string lowerExt = e;
            std::transform(lowerExt.begin(), lowerExt.end(), lowerExt.begin(),
                           [](unsigned char c) { return std::tolower(c); });

            extToType[lowerExt] = type;
        }
    }

    // Iterate over directory entries
    for (const auto &entry : fs::directory_iterator(directoryPath))
    {
        if (!entry.is_regular_file())
            continue;

        fs::path src = entry.path();
        std::string ext = src.extension().string();

        std::string lowerExt = ext;
        std::transform(lowerExt.begin(), lowerExt.end(), lowerExt.begin(),
                       [](unsigned char c) { return std::tolower(c); });

        // Dangerous files check
        if (std::find(dangerous.begin(), dangerous.end(), lowerExt) != dangerous.end())
        {
            std::cerr << RED << "Skipped dangerous file: "
                      << src.filename().string() << RESET << "\n";
            ++skipped;
            skippedFiles.push_back(src.filename());
            continue;
        }

        // Determine file type; default to "Other"
        std::string type = "Other";
        if (extToType.count(lowerExt))
            type = extToType[lowerExt];

        fs::path destDir = directoryPath / type;
        fs::path dest = destDir / src.filename();

        // Create target directory if necessary
        if (!fs::exists(destDir))
        {
            std::error_code ec;
            if (!fs::create_directory(destDir, ec))
            {
                std::cerr << RED << "Warning: Could not create directory "
                          << destDir << ": " << ec.message() << RESET << "\n";
                ++skipped;
                skippedFiles.push_back(src.filename());
                continue;
            }
        }

        // Skip if destination file already exists to avoid overwriting
        if (fs::exists(dest))
        {
            ++skipped;
            skippedFiles.push_back(src.filename());
            continue;
        }

        // Move file, capturing error codes to avoid throwing
        std::error_code ec;
        fs::rename(src, dest, ec);

        if (ec)
        {
            std::cerr << RED << "Failed to move " << src
                      << " -> " << dest << ": "
                      << ec.message() << RESET << "\n";
            ++skipped;
            skippedFiles.push_back(src.filename());
        }
        else
        {
            ++moved;
        }
    }

    // Report results
    std::cout << GREEN << "Moved: " << moved << RESET
              << "  " << YELLOW << "Skipped: " << skipped << RESET << "\n";

    if (!skippedFiles.empty())
    {
        std::cout << "Skipped files:\n";
        for (const auto &s : skippedFiles)
            std::cout << " - " << s.string() << "\n";
    }

    // Pause for user acknowledgment before returning to menu
    std::cout << YELLOW << "Press Enter to return..." << RESET;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}