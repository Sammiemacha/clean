#pragma once
#include <filesystem>
#include <vector>
#include <string>

namespace fs = std::filesystem;

/**
 * @file cleanByName.hpp
 * @brief Declarations for cleaning files by name/token.
 *
 * The functions declared here provide the interface used by the CLI
 * to organize files based on name tokens or user-specified substrings.
 * The implementation will prompt for tokens or auto-detect tokens and
 * then move matching files into named subfolders.
 */

/**
 * @brief Organize files in a directory by name tokens.
 *
 * Scans `directoryPath` for files and groups them according to name
 * tokens (either provided interactively or auto-detected). Matching
 * files are moved into subdirectories named after the detected token.
 *
 * @param directoryPath Path to the directory to scan and organize.
 *
 * @note The implementation may consult an ignore token list to avoid
 *       grouping by common stop-words. It may also prompt the user
 *       for confirmation before moving files.
 */
void cleanFilesByName(const fs::path &directoryPath);
