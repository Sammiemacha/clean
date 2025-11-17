#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include "json.hpp"
#include "colors.hpp"

using json = nlohmann::json;

/**
 * @file dangerousExts.hpp
 * @brief Provide a list of potentially dangerous file extensions.
 *
 * Some file extensions can represent executable or macro-enabled
 * content that may be risky to move or execute automatically. This
 * header exposes a helper to load a project-local JSON override
 * (`data/dangerousExts.json`) and falls back to a built-in list when
 * the file is absent or cannot be parsed.
 */

/**
 * @brief Default fallback list of dangerous extensions.
 *
 * Extensions include common executable, script, and macro-enabled
 * document formats. Each entry contains the leading dot (e.g. ".exe").
 */
static const std::vector<std::string> DEFAULT_DANGEROUS_EXTS = {
    ".exe", ".dll", ".com", ".msi", ".bin", ".sys",
    ".bat", ".cmd", ".vbs", ".js", ".jse", ".wsf", ".wsh",
    ".ps1", ".psm1", ".sh", ".bash", ".zsh",
    ".lnk", ".inf", ".msu", ".msp",
    ".docm", ".xlsm", ".pptm",
    ".scr", ".pif", ".jar", ".reg"
};

/**
 * @brief Load the dangerous-extension list.
 *
 * Attempts to read `data/dangerousExts.json` and parse a JSON object
 * containing a `dangerousExtensions` array. If the file cannot be
 * opened or parsed, the function logs a warning to `std::cerr` and
 * returns the built-in fallback list.
 *
 * @return A vector of extensions (each including the leading dot).
 *
 * @note The function prints warnings using color constants from
 *       `colors.hpp`. This helper is inline and intended for simple
 *       use in the CLI flow.
 */
inline std::vector<std::string> getDangerousExts()
{
    try
    {
        std::ifstream file("data/dangerousExts.json");
        if (!file.is_open())
        {
            std::cerr << RED << "[Warning] Could not load dangerousExts.json. Using fallback list.\n" << RESET;
            return DEFAULT_DANGEROUS_EXTS;
        }

        json j = json::parse(file);

        if (!j.contains("dangerousExtensions"))
        {
            std::cerr << RED << "[Warning] JSON missing 'dangerousExtensions'. Using fallback.\n" << RESET;
            return DEFAULT_DANGEROUS_EXTS;
        }

        return j["dangerousExtensions"].get<std::vector<std::string>>();
    }
    catch (...)
    {
        std::cerr << RED << "[Warning] Failed to parse dangerousExts.json. Using fallback.\n" << RESET;
        return DEFAULT_DANGEROUS_EXTS;
    }
}
