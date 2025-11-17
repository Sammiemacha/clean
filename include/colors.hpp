/**
 * @file colors.hpp
 * @brief ANSI terminal color constants used by the CLI.
 *
 * This header defines a small set of ANSI escape sequence string
 * constants for coloring and styling console output. They are
 * intended for use in simple terminal UIs and logging messages.
 *
 * @note These codes are standard on most UNIX-like terminal emulators.
 *       On Windows consoles prior to Windows 10 the escape sequences
 *       may not render correctly without additional configuration.
 */
#pragma once
#include <string>

/** Reset all attributes (colors and styles). */
const std::string RESET   = "\033[0m";

/** Bold text style. */
const std::string BOLD    = "\033[1m";

/** Dim text style. */
const std::string DIM     = "\033[2m";

/** Red foreground color. */
const std::string RED     = "\033[31m";

/** Green foreground color. */
const std::string GREEN   = "\033[32m";

/** Yellow foreground color. */
const std::string YELLOW  = "\033[33m";

/** Blue foreground color. */
const std::string BLUE    = "\033[34m";

/** Magenta foreground color. */
const std::string MAGENTA = "\033[35m";

/** Cyan foreground color. */
const std::string CYAN    = "\033[36m";

/** White foreground color. */
const std::string WHITE   = "\033[37m";
