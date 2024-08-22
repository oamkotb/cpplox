/**
 * @file main.cpp
 * @brief Entry point for the cpplox interpreter.
 *
 * This file contains the main function which is the entry point for the cpplox interpreter.
 * It handles command line arguments and decides whether to run a script or enter the interactive prompt.
 */

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#include "AstPrinter.h"
#include "Interpreter.h"
#include "Parser.h"
#include "Scanner.h"
#include "Stmt.h"
#include "Token.h"
#include "utils.h"

Interpreter interpreter; // Persistent interpreter object
bool had_error = false; // Extern

/**
 * @brief Executes the scanning process on the source code.
 *
 * This function takes a string representing the source code and initializes a Scanner object with it.
 * It then scans the source code for tokens and prints each token to the standard output.
 * @param source The source code to scan.
 */
void run(const std::string& source)
{
  // Initialize the scanner with the source code.
  Scanner scanner(source);

  // Scan the source code for tokens.
  std::vector<Token> tokens;
  tokens = scanner.scanTokens();

  // Parse the tokens.
  Parser<LiteralValue> parser(tokens);
  std::vector<std::shared_ptr<Stmt<LiteralValue>>> statements = parser.parse();

  if (had_error) return;
  
  // Interpret the expression.
  interpreter.interpret(statements);
}

/**
 * @brief Runs a script from a given file path.
 * 
 * @param path The file path to the script that needs to be run.
 */
void runFile(const std::string& path)
{
  // Attempt to open the script file.
  std::ifstream script(path);
  if (!script)
  {
    std::cerr << "Error opening file: " << path << std::endl;
    had_error = true;
    return;
  }

  // Try to read the file contents into a string.
  try
  {
    std::stringstream buffer;
    buffer << script.rdbuf();
    std::string source = buffer.str();
    script.close();

    // Run the script using the source string.
    run(source);
  }
  catch (const std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << std::endl;
    had_error = true;
    return;
  }
}

/**
 * @brief Enters the interactive prompt mode.
 *
 * This function initiates a loop that continuously prompts the user for input.
 * It reads a line from the standard input and executes it. If an empty line is encountered,
 * it is ignored. The loop ends when EOF is reached.
 */
void runPrompt()
{
  std::cout << "> ";
  
  std::string line;
  while (std::getline(std::cin, line))
  {
    // Ignore empty lines.
    if (line.empty()) continue;

    // Execute command and continue even on encountering errors.
    run(line);

    had_error = false;

    // End loop on end-of-file.
    if (std::cin.eof()) break; 
    
    std::cout << "> ";
  }
}

/**
 * @brief Main function.
 * @param argc Number of command line arguments.
 * @param argv Array of command line argument strings.
 * @return Returns EXIT_SUCCESS on successful execution, otherwise returns EXIT_FAILURE.
 */
int main(int argc, char* argv[])
{
  // Flag to indicate if an error has occurred.
  had_error = false;

  /**
   * Incorrect usage: too many command line arguments.
   */
  if (argc > 2)
  {
    std::cout << "Usage: cpplox [script]" << std::endl;
    return EXIT_FAILURE;
  }
  /**
   * Correct usage: one argument, run the script file.
   */
  else if (argc == 2)
  {
    runFile(argv[1]);
    if (had_error) return EXIT_FAILURE;
  }
  /**
   * Correct usage: no arguments, run in interactive mode.
   */
  else
    runPrompt();

  return EXIT_SUCCESS;
}