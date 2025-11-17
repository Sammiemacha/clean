#include "fileTypes.hpp"
#include "json.hpp"
#include <fstream>
#include <iostream>

/**
 * @file fileTypes.cpp
 * @brief Loads and provides mappings of file-type categories to file extensions.
 *
 * The module attempts to read `data/filetypes.json` for a user-provided
 * mapping of category → extension list. If the JSON file is missing or
 * invalid the implementation falls back to a built-in mapping returned by
 * `fallbackFileTypes()`.
 *
 * The `getFileTypes()` function provides a cached, read-once accessor that
 * loads the mappings on first call.
 */

using json = nlohmann::json;

/**
 * @brief In-memory cache of loaded file type mappings.
 *
 * Populated on first call to `getFileTypes()` by `loadFileTypes()`.
 * This cache is intentionally single-threaded; callers in multi-threaded
 * contexts should add synchronization as needed.
 */
static std::map<std::string, std::vector<std::string>> FILE_TYPES_CACHE;

/**
 * @brief Indicates whether the cache has been populated.
 */
static bool loaded = false;

// -----------------------
// Hardcoded fallback map
// -----------------------
/**
 * @brief Returns a built-in mapping of category → extension list.
 *
 * This fallback is used when `data/filetypes.json` cannot be opened or is
 * invalid JSON. The mapping includes common media, document, archive and code
 * extensions and provides a sensible default categorization.
 *
 * @return std::map<std::string, std::vector<std::string>> The fallback mapping.
 */
static std::map<std::string, std::vector<std::string>> fallbackFileTypes()
{
    return {
        {"Images", {".jpg", ".jpeg", ".png", ".gif", ".bmp", ".tiff", ".webp", ".heic", ".heif", ".svg", ".ico", ".jfif", ".raw", ".arw", ".cr2", ".nef", ".orf", ".dng"}},
        {"Videos", {".mp4", ".mov", ".avi", ".mkv", ".wmv", ".flv", ".webm", ".mpeg", ".mpg", ".3gp", ".m4v", ".ts", ".mts", ".vob"}},
        {"Audio", {".mp3", ".wav", ".flac", ".aac", ".ogg", ".wma", ".m4a", ".opus", ".aiff", ".mid", ".midi"}},
        {"Documents", {".pdf", ".doc", ".docx", ".txt", ".rtf", ".odt", ".csv", ".xlsx", ".xls", ".ppt", ".pptx", ".epub", ".md", ".tex", ".pages", ".numbers", ".key"}},
        {"Archives", {".zip", ".rar", ".7z", ".tar", ".gz", ".bz2", ".xz", ".iso", ".dmg", ".tgz", ".cab"}},
        {"Code", {".py", ".js", ".html", ".css", ".c", ".cpp", ".h", ".hpp", ".java", ".sh", ".ts", ".php", ".rb", ".go", ".swift", ".kt", ".rs", ".lua", ".sql", ".json", ".xml", ".yml", ".yaml", ".cs", ".vb", ".pl", ".asm", ".bat", ".cmd"}},
        {"Fonts", {".ttf", ".otf", ".woff", ".woff2", ".eot", ".fon"}},
        {"3D_Models", {".obj", ".fbx", ".stl", ".blend", ".3ds", ".dae", ".ply", ".gltf", ".glb"}},
        {"Subtitles", {".srt", ".vtt", ".ass", ".ssa", ".sub"}},
        {"Configs", {".ini", ".cfg", ".conf", ".jsonc", ".toml", ".env", ".properties"}},
        {"DiskImages", {".iso", ".img", ".vhd", ".vhdx", ".vdi", ".vmdk"}},
        {"Packages", {".deb", ".rpm", ".apk", ".jar", ".whl", ".gem", ".msi"}},
        {"Other", {}}
    };
}

// -----------------------
// Try reading JSON
// -----------------------
/**
 * @brief Loads file type mappings from `data/filetypes.json`.
 *
 * Attempts to open and parse the JSON file. The expected format is a top-level
 * object where keys are category names and values are arrays of extension
 * strings (including the leading dot), e.g.:
 * {
 *   "Images": [".jpg",".png"],
 *   "Documents": [".pdf",".docx"]
 * }
 *
 * If the file cannot be opened or parsed, the function returns the
 * `fallbackFileTypes()` mapping.
 *
 * @return std::map<std::string, std::vector<std::string>> Loaded mapping.
 */
std::map<std::string, std::vector<std::string>> loadFileTypes()
{
    std::ifstream f("data/filetypes.json");

    if (!f.is_open())
    {
        std::cerr << "[WARN] Could not open data/filetypes.json, using fallback.\n";
        return fallbackFileTypes();
    }

    try
    {
        json j;
        f >> j;

        std::map<std::string, std::vector<std::string>> result;

        for (auto &[key, value] : j.items())
        {
            result[key] = value.get<std::vector<std::string>>();
        }

        return result;
    }
    catch (...)
    {
        std::cerr << "[WARN] Invalid JSON in filetypes.json, using fallback.\n";
        return fallbackFileTypes();
    }
}

// -----------------------
// Public accessor
// -----------------------
/**
 * @brief Returns the cached mapping of file types to extensions.
 *
 * On first invocation, the function loads the mapping from
 * `data/filetypes.json` (falling back to the built-in mapping on error)
 * and caches the result for subsequent calls.
 *
 * @return const std::map<std::string, std::vector<std::string>>& Reference to cached mapping.
 */
const std::map<std::string, std::vector<std::string>>& getFileTypes()
{
    if (!loaded)
    {
        FILE_TYPES_CACHE = loadFileTypes();
        loaded = true;
    }
    return FILE_TYPES_CACHE;
}
