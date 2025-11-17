#pragma once
#include <iostream>

/**
 * @file header.hpp
 * @brief Declaration of the Header utility for displaying the application banner.
 *
 * The `Header` class provides a simple static helper to render the ASCII
 * application title and decorative formatting in the terminal. The
 * implementation is provided in `src/header.cpp`.
 */
class Header {
public:
    /**
     * @brief Render the application banner and formatting to stdout.
     *
     * This function clears the terminal (platform-specific) and prints an
     * ASCII-art title with color formatting. It is intended for interactive
     * TUI use and performs console output only.
     *
     * @note Implementation is in `src/header.cpp`.
     */
    static void display();
};