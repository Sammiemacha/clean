/**
 * @file fileTypes.hpp
 * @brief Load and provide access to file-type → extensions mapping.
 *
 * The project stores a mapping from human-friendly file type names
 * (e.g. "Images", "Code") to a list of associated file extensions
 * (e.g. "jpg", "cpp"). The implementation loads this mapping from
 * a JSON data file (see `data/fileTypes.json`) with a fallback hard-
 * coded map when the file is absent or malformed.
 */
#pragma once

#include <map>
#include <vector>
#include <string>

/**
 * @brief Load file type mappings from disk (or fallback).
 *
 * Reads `data/fileTypes.json` and constructs a map where the key is the
 * file type name and the value is a vector of extensions (extensions are
 * expected without a leading dot). If the JSON file cannot be read or
 * parsed, the function returns a sensible built-in fallback mapping.
 *
 * @return A map from file type name to a vector of extensions.
 *
 * @note The returned map is by-value to allow callers to take ownership
 *       if needed; the library also provides `getFileTypes()` which
 *       returns a cached reference for cheap lookups.
 */
std::map<std::string, std::vector<std::string>> loadFileTypes();

/**
 * @brief Get the (cached) file type mapping.
 *
 * Returns a const reference to an internal, cached mapping that is
 * populated on first use by calling `loadFileTypes()`. This avoids
 * repeatedly parsing the JSON file on every lookup.
 *
 * @return Const reference to the file type → extensions map.
 *
 * @warning This function is not synchronized for multi-threaded use.
 *          If your program uses threads, protect access or modify the
 *          implementation to add locking.
 */
const std::map<std::string, std::vector<std::string>>& getFileTypes();
