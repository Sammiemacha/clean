/**
 * @file cleanByName.cpp
 * @brief Implements name-based file organization utilities.
 *
 * Provides interactive logic to move files into folders based on either a
 * user-provided search string (substring match against filenames) or
 * auto-detected common tokens found in file stems. Uses dynamic ignore
 * tokens from `getIgnoreTokens()` and reports moved/skipped counts.
 */

#include <iostream>
#include <string>
#include <filesystem>
#include <vector>
#include <unordered_set>
#include <map>
#include <algorithm>
#include <limits>
#include <cctype>

#include "../include/colors.hpp"
#include "../include/ignoreTokens.hpp"

namespace fs = std::filesystem;
using namespace std;


// Forward declarations
void byName(const fs::path &directoryPath);

/**
 * @brief Convert a string to lowercase.
 *
 * Helper lambda used throughout the module to perform case-insensitive
 * comparisons and token extraction. Accepts ASCII/UTF-8 bytes and uses
 * `std::tolower` safely via `unsigned char` conversion.
 *
 * @param s Input string
 * @return std::string Lowercased copy of `s`
 */
auto toLower = [](const string &s) {
    string out = s;
    transform(out.begin(), out.end(), out.begin(), [](unsigned char c) {
        return tolower(c);
    });
    return out;
};


/**
 * @brief Organize files by name.
 *
 * If the user provides a non-empty name, the function finds files whose
 * filenames contain that substring (case-insensitive) and moves them into a
 * directory named after the provided string (slashes are sanitized to
 * underscores).
 *
 * If the user just presses Enter, the function attempts to auto-detect
 * common tokens within file stems (tokens >= 4 characters, excluding
 * tokens from `getIgnoreTokens()`). Tokens that appear at least twice are
 * considered and the top tokens (up to 10) are used to group files into
 * token-named directories.
 *
 * The function prints a summary of moved/skipped files and pauses for
 * user acknowledgment before returning (interactive TUI behavior).
 *
 * @param directoryPath Directory to scan and organize.
 *
 * @note Files that would overwrite existing files in the destination are
 *       skipped. Any filesystem errors are reported and the offending file
 *       is skipped.
 */
void cleanFilesByName(const fs::path &directoryPath)
{
    // Clear any leftover newline from prior input and prompt the user
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << BOLD << WHITE << "Enter a name to search for in filenames (press Enter to auto-detect common names):\n>>: " << RESET;
    string name;
    getline(cin, name);

    vector<fs::path> matches;      // Files matching the name
    size_t moved = 0;              // Count of moved files
    size_t skipped = 0;            // Count of skipped files
    vector<fs::path> skippedFiles; // List of skipped files

    // Branch 1: explicit user-supplied name search
    if (!name.empty())
    {
        string needle = toLower(name);
        for (const auto &entry : fs::directory_iterator(directoryPath))
        {
            if (!entry.is_regular_file())
                continue;
            string fname = entry.path().filename().string();
            if (toLower(fname).find(needle) != string::npos)
            {
                matches.push_back(entry.path());
            }
        }

        if (matches.empty())
        {
            cout << YELLOW << "No files found containing '" << name << "'.\n" << RESET;
            cout << YELLOW << "Press Enter to return to the menu..." << RESET;
            cin.get();
            return;
        }

        // Create destination directory named after provided name (sanitize)
        string dirName = name;
        for (auto &c : dirName)
            if (c == '/' || c == '\\')
                c = '_'; // replace slashes with underscores
        
        fs::path destDir = directoryPath / dirName;
        error_code ec;
        if (!fs::exists(destDir))
            fs::create_directory(destDir, ec);
        if (ec)
        {
            cerr << RED << "Failed to create directory '" << destDir << "': " << ec.message() << RESET << "\n";
            return;
        }

        // Move matched files
        for (const auto &src : matches)
        {
            fs::path dest = destDir / src.filename();
            if (fs::exists(dest))
            {
                ++skipped;
                skippedFiles.push_back(src.filename());
                continue;
            }
            error_code moveEc;
            fs::rename(src, dest, moveEc);
            if (moveEc)
            {
                ++skipped;
                skippedFiles.push_back(src.filename());
            }
            else
            {
                ++moved;
            }
        }
    }
    else
    {
        // Auto-detect common name tokens
        map<string, int> tokenCount;// Token frequency map
        
        // Build ignore set from dynamic tokens
        const auto& ignoreTokensVec = getIgnoreTokens(); // from include/ignoreTokens.hpp
        unordered_set<string> ignoreSet(ignoreTokensVec.begin(), ignoreTokensVec.end()); 

        // Tokenize stems and count tokens of length >= 4
        for (const auto &entry : fs::directory_iterator(directoryPath))
        { //loop through files
            if (!entry.is_regular_file()){// Check for regular file
                cout<< DIM << "Skipping non-regular file: " << entry.path().filename().string() << RESET << "\n";
                continue;
            }
            string stem = entry.path().stem().string();
            string lower = toLower(stem);// Lowercase stem
            
            // Split on non-alphanumeric
            string token;
            for (char ch : lower)
            {
                if (isalnum(static_cast<unsigned char>(ch)))// Alphanumeric check
                {
                    token.push_back(ch);
                }
                else
                { 
                    if (token.size() >= 4 && ignoreSet.find(token) == ignoreSet.end()){
                        tokenCount[token]++;
                        token.clear();
                    }
                }
            }
            
            // Handle final token
            if (token.size() >= 4 && ignoreSet.find(token) == ignoreSet.end()){
                tokenCount[token]++;
            }
            // Also count whole stem if long enough
            if (lower.size() >= 4 && ignoreSet.find(lower) == ignoreSet.end()){
                tokenCount[lower]++;
            }
        }

        // Collect tokens that appear multiple times
        vector<pair<string, int>> common;
        for (auto &kv : tokenCount)
            if (kv.second >= 2)
                common.emplace_back(kv.first, kv.second);
        
        // Sort by frequency descending
        sort(common.begin(), common.end(), [](auto &a, auto &b) {
            return a.second > b.second;
        });

        if (common.empty())
        {
            cout << YELLOW << "No common name tokens detected. Nothing to move.\n" << RESET;
            cout << YELLOW << "Press Enter to return to the menu..." << RESET;
            cin.get();
            return;
        }

        // Limit to top 10 tokens to avoid over-creating folders
        size_t limit = min<size_t>(10, common.size());
        for (size_t i = 0; i < limit; ++i)
        {
            const string token = common[i].first;
            
            // Find files that contain this token
            vector<fs::path> found;
            for (const auto &entry : fs::directory_iterator(directoryPath))
            {
                if (!entry.is_regular_file())
                    continue;
                string fnameLower = toLower(entry.path().filename().string());
                if (fnameLower.find(token) != string::npos && ignoreSet.find(token) == ignoreSet.end())
                    found.push_back(entry.path());// Add to found list
            }

            if (found.size() < 2)
                continue; // Skip tokens that don't represent groups

            // Make directory named after token
            fs::path destDir = directoryPath / token;
            error_code ec;
            if (!fs::exists(destDir))
                fs::create_directory(destDir, ec);
            if (ec)
            {
                cerr << RED << "Failed to create directory '" << destDir << "': " << ec.message() << RESET << "\n";
                continue;
            }

            // Move files for this token
            for (const auto &src : found)
            {
                fs::path dest = destDir / src.filename();
                if (fs::exists(dest))
                {
                    ++skipped;
                    cout<< DIM << "Skipping file due to name conflict: " << src.filename().string() << RESET << "\n";
                    skippedFiles.push_back(src.filename());
                    continue;
                }
                error_code moveEc;
                fs::rename(src, dest, moveEc);
                if (moveEc)
                {
                    ++skipped;
                    cout<< DIM << "Skipping file due to error: " << src.filename().string() << RESET << "\n";
                    skippedFiles.push_back(src.filename());
                }
                else
                {
                    ++moved;
                }
            }
        }
    }

    // Print results
    cout << GREEN << "Moved: " << moved << RESET << "  " << YELLOW << "Skipped: " << skipped << RESET << "\n";
    if (!skippedFiles.empty())
    {
        cout << DIM << "Skipped files (name conflicts or errors):\n" << RESET;
        for (const auto &s : skippedFiles)
            cout << " - " << s.string() << "\n";
    }

    cout << YELLOW << "Press Enter to return to the menu..." << RESET;
    cin.get();

    return;
}