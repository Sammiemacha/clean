#pragma once
#include <filesystem>
#include <string>
#include <vector>
#include <map>

namespace fs = std::filesystem;

/**
 * @file cleanByType.hpp
 * @brief Declarations for cleaning files by their detected type.
 *
 * These declarations expose the interface used by the CLI to organize
 * files into type-based subdirectories (e.g. Images, Documents).
 * The implementation consults the file type mapping and may perform
 * additional checks (such as dangerous extension filtering) before
 * moving files.
 */

/**
 * @brief Organize files in a directory by type.
 *
 * Scans the provided `directoryPath` and groups files into subfolders
 * according to the file type mapping returned by `getFileTypes()`.
 * The function may skip files with extensions considered dangerous
 * (see `getDangerousExts()`) and will create target subdirectories
 * as needed.
 *
 * @param directoryPath Path to the directory to scan and organize.
 *
 * @note The function may output progress or warnings to `std::cout`
 *       / `std::cerr` and uses color constants from `colors.hpp`.
 */
void cleanFilesByType(const fs::path &directoryPath);
