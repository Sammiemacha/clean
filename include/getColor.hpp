#pragma once
#include <string>
#include <algorithm>
#include "colors.hpp"
#include "fileTypes.hpp"

/**
 * @file getColor.hpp
 * @brief Map file extensions to ANSI color constants.
 *
 * This header provides an inline helper `getColorForExtension` which
 * inspects the file type groups returned by `getFileTypes()` (see
 * `fileTypes.hpp`) and returns an ANSI color code string defined in
 * `colors.hpp` appropriate for the extension.
 *
 * @note The function expects an extension without a leading dot (e.g., "jpg").
 * @note This header is intentionally header-only and uses an `inline`
 * function for easy inclusion across translation units.
 */

/**
 * @brief Returns an ANSI color string for a file extension.
 *
 * Searches the file type -> extension mapping provided by
 * `getFileTypes()` and returns a color constant from `colors.hpp`.
 * Common mappings:
 * - Images -> `GREEN`
 * - Videos -> `MAGENTA`
 * - Audio -> `CYAN`
 * - Documents -> `YELLOW`
 * - Archives -> `RED`
 * - Code -> `BLUE`
 *
 * @param ext File extension (without a leading dot), case-sensitive as stored
 *            in the file type map.
 * @return ANSI color escape string constant. Returns `WHITE` when the
 *         extension is not found in any known file type group.
 */
inline std::string getColorForExtension(const std::string &ext)
{
    for (const auto &[type, exts] : getFileTypes())
    {
        if (std::find(exts.begin(), exts.end(), ext) != exts.end())
        {
            if (type == "Images")
                return GREEN;
            if (type == "Videos")
                return MAGENTA;
            if (type == "Audio")
                return CYAN;
            if (type == "Documents")
                return YELLOW;
            if (type == "Archives")
                return RED;
            if (type == "Code")
                return BLUE;
        }
    }
    return WHITE; // Default color
}
