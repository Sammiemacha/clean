#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
#include "json.hpp"

using json = nlohmann::json;
using namespace std;
/**
 * @file ignoreTokens.hpp
 * @brief Provides a list of tokens to ignore when auto-detecting common name tokens.
 *
 * The module exposes two helpers:
 * - `defaultIgnoreTokens()` returns a built-in, hardcoded list of tokens.
 * - `getIgnoreTokens()` attempts to load additional tokens from
 *   `data/ignoreTokens.json` (key `ignoreTokens`) and falls back to the
 *   built-in list when the file is missing or invalid. Tokens are
 *   normalized to lowercase and cached on first access.
 */

// -------------------------
// Default hardcoded tokens
// -------------------------
/**
 * @brief Returns the built-in default list of ignore tokens.
 *
 * The returned reference points to a function-local static vector and is
 * safe to use for the lifetime of the program.
 *
 * @return const std::vector<std::string>& Reference to the built-in ignore tokens.
 */
inline const std::vector<std::string>& defaultIgnoreTokens()
{
    static const std::vector<std::string> TOKENS = {
        "official", "lyrics", "video", "audio", "hd", "remix", "mv", "live", "youtube",
        "ft", "feat", "2025", "720p", "1080", "1080p", "best", "song", "songs", "360p", "featuring",
        "www", "com", "net", "org", "sample", "256k", "season", "episode", "lyric", "music"
    };
    return TOKENS;
}

// -----------------------------------------
// Load Ignore Tokens from JSON file or fallback
// -----------------------------------------
/**
 * @brief Returns the ignore tokens loaded from `data/ignoreTokens.json` or a fallback.
 *
 * On first call this function attempts to open `data/ignoreTokens.json` and
 * read an array at key `ignoreTokens`. If the file is missing, malformed, or
 * does not contain the expected array, the function falls back to
 * `defaultIgnoreTokens()`.
 *
 * The loaded list is normalized to lowercase and cached for subsequent calls.
 * The function is not synchronized for multi-threaded access; if the program
 * uses threads it should ensure external synchronization when calling this
 * function during startup.
 *
 * @return const std::vector<std::string>& Reference to the cached ignore tokens.
 */
inline const std::vector<std::string>& getIgnoreTokens()
{
    static bool loaded = false;
    static std::vector<std::string> tokens;

    if (!loaded)
    {
        loaded = true; // prevent reloading

        std::ifstream file("data/ignoreTokens.json");
        if (!file.is_open())
        {
            // JSON file missing → fallback silently
            tokens = defaultIgnoreTokens();
            cout <<DIM<< "[INFO] ignoreTokens.json not found, using default ignore tokens.\n"<< RESET;
            return tokens;
        }

        try
        {
            json j;
            file >> j;

            if (!j.contains("ignoreTokens") || !j["ignoreTokens"].is_array())
            {
                // JSON structure wrong → fallback
                tokens = defaultIgnoreTokens();
                cout <<DIM<< "[INFO] ignoreTokens.json invalid format, using default ignore tokens.\n"<< RESET;
                return tokens;
            }

            for (const auto& t : j["ignoreTokens"])
                tokens.push_back(t.get<std::string>());

            // normalize to lowercase for safety
            for (auto& t : tokens)
                std::transform(t.begin(), t.end(), t.begin(), ::tolower);
        }
        catch (...)
        {
            // Invalid JSON → fallback
            cout <<DIM<< "[INFO] ignoreTokens.json invalid JSON, using default ignore tokens.\n"<< RESET;
            tokens = defaultIgnoreTokens();
        }
    }

    return tokens;
}
