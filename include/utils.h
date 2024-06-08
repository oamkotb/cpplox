#pragma once

#include <string>

/** 
 * @file utils.h
 * @brief Utility functions for error reporting in the Lox interpreter.
 */

namespace Lox
{
    /** Flag to indicate if an error has occurred. */
    extern bool had_error;

    void error(int line, std::string message);

    /**
     * @brief Prints an error message to standard error stream.
     * 
     * This function sets the had_error flag and prints an error message
     * formatted with the line number and location of the error.
     * 
     * @param line The line number where the error occurred.
     * @param where Additional information about the error location.
     * @param message The error message to be reported.
     */
    void report(int line, std::string where, std::string message);
}
