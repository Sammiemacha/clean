/**
 * @file main.cpp
 * @brief Main entry point and terminal user interface (TUI) for the file organization utility.
 *
 * This module provides the primary menu system and navigation logic for organizing
 * files by type or by common name patterns. It handles user input validation,
 * menu rendering, and delegates to specialized cleaning functions.
 *
 * @author Samuel Njuguna
 * @version 1.0.0
 * @date November 2025
 */

#include <iostream>     ///< Standard input/output streams
#include <string>       ///< String class for text manipulation
#include <filesystem>   ///< Filesystem operations (C++17)


/// @cond Internal headers
#include "../include/header.hpp"          ///< Application header display utilities
#include "../include/colors.hpp"          ///< ANSI color codes for terminal output
#include "../include/fileTypes.hpp"       ///< File type definitions and utilities
#include "../include/listFiles.hpp"       ///< Directory file listing utilities
#include "../include/getColor.hpp"        ///< Color selection utilities
#include "../include/getDirectory.hpp"    ///< Directory path input utilities
#include "../include/clean/cleanByType.hpp"    ///< File cleaning by type implementation
#include "../include/clean/cleanByName.hpp"    ///< File cleaning by name implementation
#include "../include/dangerousExts.hpp"       ///< Dangerous file extension detection
/// @endcond

namespace fs = std::filesystem;  ///< Alias for std::filesystem namespace
using namespace std;             ///< Using standard namespace

/**
 * @brief Clears the console screen in a platform-independent manner.
 *
 * Uses system-specific commands:
 * - Windows: `cls` command
 * - Unix/Linux/macOS: `clear` command
 *
 * @warning This function uses system() which may not be available in all
 *          restricted environments. Consider using platform-specific libraries
 *          for production code.
 */
void clearScreen();

/**
 * @brief Displays the main operation menu and handles user selection.
 *
 * Presents a menu with options to:
 * - Exit the program (0)
 * - Clean files by type (1)
 * - Clean files by name (2)
 *
 * Validates input and loops indefinitely until user chooses to exit.
 *
 * @details The function handles input errors gracefully, clearing the input
 *          buffer and prompting the user to retry on invalid selections.
 *
 * @see byType()
 * @see byName()
 */
void operation();

/**
 * @brief Provides the Terminal User Interface (TUI) for cleaning files by type.
 *
 * @param directoryPath The filesystem path to the target directory.
 *
 * @details Presents a submenu with options to:
 * - Return to main menu (0)
 * - List all files in the directory (1)
 * - Execute the file cleaning operation (2) - organizes into type-based subdirectories
 * - Change to a different directory (3)
 *
 * The function is recursive and will re-display the menu after each operation.
 *
 * @see cleanFilesByType()
 * @see listFilesInDirectory()
 */
void byType(const fs::path &directoryPath);

/**
 * @brief Provides the Terminal User Interface (TUI) for cleaning files by name.
 *
 * @param directoryPath The filesystem path to the target directory.
 *
 * @details Presents a submenu similar to byType() but for name-based organization.
 * Options include:
 * - Return to main menu (0)
 * - List all files in the directory (1)
 * - Execute the file cleaning operation (2) - organizes into name-based subdirectories
 * - Change to a different directory (3)
 *
 * The function is recursive and will re-display the menu after each operation.
 *
 * @see cleanFilesByName()
 * @see listFilesInDirectory()
 */
void byName(const fs::path &directoryPath);

/**
 * @brief Application entry point.
 *
 * @return int Exit status code (0 on success).
 *
 * @details Initializes the application by calling the main operation menu.
 *          The program remains in the menu loop until the user chooses to exit.
 */
int main(){

    operation();

    return 0;
}

// Clear the console screen
void clearScreen(){
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

/**
 * @brief Main terminal user interface (TUI) operation loop.
 *
 * Displays a menu to the user and handles their selection for file organization operations.
 * Continues to loop until the user selects the exit option.
 */
void operation(){
    bool isRunning = true;
    int option = -1;
    std::string directoryPath;

    do
    {
        Header::display();
        do
        {
            cout << BOLD << MAGENTA << "Choose an operation:\n"
                      << RESET;
            cout << WHITE << "0. Exit\n"
                      << RESET << BLUE << "-----\n"
                      << RESET << WHITE << "1. clean by type\n";
            cout << "2. clean by name\n";
            cout << RESET;
            cout << YELLOW << ">>: " << RESET << GREEN;
            std::cin >> option;
            cout << RESET;
            if (std::cin.fail())
            {
                // Handle invalid input (non-numeric)
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                cout << RED << "Invalid input. Please enter a number between 0 and 2.\n\n"<< RESET;
                cout << YELLOW << "Press Enter to try again..." << RESET;
                std::cin.get();
                Header::display();
                option = -1;
                continue;
            }
            if (option < 0 || option > 2){
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                cout << RED << "Invalid input. Please enter a number between 0 and 2.\n\n"<< RESET;
                cout << YELLOW << "Press Enter to try again..." << RESET;
                std::cin.get();
                Header::display();
            }
        } while (option < 0 || option > 2);

        switch (option){
        case 0:
            cout << GREEN << "Good Bye" << RESET << endl;
            exit(0);// Exit program
            break;
        case 1:
            // Get directory path from user and clean by type
            directoryPath = getDirectoryPath();
            byType(directoryPath);
            break;
        case 2:
            // Get directory path from user and clean by name
            directoryPath = getDirectoryPath();
            byName(directoryPath);
            break;
        default:
            cout << "Invalid choice. Please enter a number between 0 and 2.\n";
        }
    } while (isRunning);
}

/**
 * @brief Terminal User Interface for organizing files by type.
 *
 * Displays a submenu allowing users to view files in a directory or organize
 * them into type-based subdirectories. Includes options to change directory
 * or return to the main menu.
 *
 * @param directoryPath The path to the directory to organize.
 */
void byType(const fs::path &directoryPath){
    int option = -1;
    do{
        /**
         * Display the byType submenu with numbered options for user selection.
         * Options 0-3 are validated in the loop condition.
         */
        Header::display();
        cout << WHITE << "0. Return to main menu\n"
        << RESET << BLUE << "-----\n"
        << RESET << "1. List all files in the directory" << endl;
            cout << "2. clean files into subdirectories by type\n"
        << RESET << BLUE << "-----\n"
        << RESET << "3. Change Directory" << endl;
        cout << YELLOW << ">>: " << GREEN;
        std::cin >> option;
        cout << RESET;
        if (std::cin.fail()){
            // Handle invalid numeric input
            std::cin.clear();
            clearScreen();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            cout << RED << "Invalid input. Please enter a number between 0 and 3.\n\n"
                      << RESET;
            option = -1;
            continue;
        }
    } while (option < 0 || option > 3);
    switch (option)
    {
    case 0:
        // Return to main operation menu
        operation();
        break;
    case 1:
        // List all files in the current directory
        listFilesInDirectory(directoryPath);
        byType(directoryPath);
        break;
    case 2:
        // Execute the file cleaning operation by type
        cleanFilesByType(directoryPath);
        byType(directoryPath);
        break;
    case 3:
    {
        // Get new directory path and continue in byType menu
        std::string directory = getDirectoryPath();
        byType(directory);
        break;
    }
    default:
        cout << "Invalid choice. Please enter a number between 0 and 3.\n";
        break;
    }
}

/**
 * @brief Terminal User Interface for organizing files by name.
 *
 * Displays a submenu allowing users to view files in a directory or organize
 * them into name-based subdirectories. Includes options to change directory
 * or return to the main menu.
 *
 * @param directoryPath The path to the directory to organize.
 *
 * @note This TUI delegates to cleanFilesByName() which supports both user-specified
 *       and auto-detected naming patterns.
 */
void byName(const fs::path &directoryPath)
{
    int option = -1;
    Header::display();
    
    // Validate that the provided directory path exists and is a directory
    if (!fs::exists(directoryPath) || !fs::is_directory(directoryPath))
    {
        std::cerr << RED << "Invalid directory provided.\n"
                  << RESET;
    }

    do
    {
        /**
         * Display the byName submenu with numbered options for user selection.
         * Options 0-3 are validated in the loop condition.
         */
        Header::display();
        cout << WHITE << "0. Return to main menu\n"
                  << RESET << BLUE << "-----\n"
                  << RESET << "1. List all files in the directory" << endl;
        cout << "2. clean files into subdirectories by name\n"
                  << RESET << BLUE << "-----\n"
                  << RESET << "3. Change Directory" << endl;
        cout << YELLOW << ">>: " << GREEN;
        std::cin >> option;
        cout << RESET;
        if (std::cin.fail())
        {
            // Handle invalid numeric input
            std::cin.clear();
            clearScreen();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            cout << RED << "Invalid input. Please enter a number between 0 and 3.\n\n"<< RESET;
            option = -1;
            continue;
        }
    } while (option < 0 || option > 3);

    switch (option)
    {
    case 0:
        // Return to main operation menu
        operation();
        break;
    case 1:
        // List all files in the current directory
        listFilesInDirectory(directoryPath);
        byName(directoryPath); // Redisplay the byName menu
        break;
    case 2:
        // Execute the file cleaning operation by name
        cleanFilesByName(directoryPath);
        byName(directoryPath);// Redisplay the byName menu
        break;
    case 3:
    {
        // Get new directory path and continue in byName menu
        std::string directory = getDirectoryPath();
        byName(directory);// Redisplay the byName menu
        break;
    }
    default:
        cout << "Invalid choice. Please enter a number between 1 and 3.\n";
        break;
    }
}
