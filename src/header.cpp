/**
 * @file header.cpp
 * @brief Implementation of the Header class for displaying application title and formatting.
 *
 * This module provides the Header class which displays a formatted ASCII title banner
 * for the file organization application. The display is platform-aware and includes
 * decorative elements with terminal color support.
 *
 * @see header.hpp
 */

#include "header.hpp"  ///< Header class declaration
#include "colors.hpp"  ///< ANSI color code macros

/**
 * @brief Displays the application title banner with ASCII art and formatting.
 *
 * Clears the console screen and displays a formatted ASCII art title with:
 * - Application name "Clean" in large ASCII letters
 * - Platform-specific screen clearing (Windows `cls` or Unix `clear`)
 * - Color formatting using ANSI codes (cyan bold title, dim separator line)
 * - Decorative separator line
 *
 * @details The function uses conditional compilation to handle platform differences:
 * - On Windows: executes `cls` command
 * - On Unix/Linux/macOS: executes `clear` command
 *
 * The ASCII art banner is defined as a raw string literal to preserve exact formatting.
 *
 * @warning Uses system() function which may not be available in restricted environments.
 *          Consider using platform-specific terminal libraries for production code.
 *
 * @see colors.hpp for ANSI color code definitions (BOLD, CYAN, RESET, DIM)
 */
void Header::display(){
    // Clear terminal screen in a platform-specific manner
    #ifdef _WIN32
            system("cls");        ///< Windows command to clear screen
    #else
            system("clear");      ///< Unix/Linux/macOS command to clear screen
    #endif
    
    // ASCII art title for the "Clean" application
    // Using raw string literal to preserve exact spacing and special characters
    const char *asciiTitle =
        R"(             
             _____   _                     
            / ____| | |                   
            | |     | | ___  __ _ _ __   
            | |     | |/ _ \/ _` | '_ \  
            | |___  | |  __/ (_| | | | | 
            \_____| |_|\___|\__,_|_| |_| 
                    )";
    
    // Display the ASCII title with bold cyan color formatting
    std::cout << BOLD << CYAN << asciiTitle << RESET << std::endl;
    
    // Display a decorative separator line in dim gray color
    std::cout << DIM << "---------------------------------------" << RESET << std::endl;
}