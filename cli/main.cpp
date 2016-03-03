/**
 *
 * @mainpage Clint
 * @version 1.66
 *
 * @section overview_sec Overview
 * Clint is a simple tool for static analysis of C/C++ code.
 *
 * When you write a checker you have access to:
 *  - %Token list = the tokenized code
 *  - Syntax tree = Syntax tree of each expression
 *  - SymbolDatabase = Information about all types/variables/functions/etc
 *    in the current translation unit
 *  - Library = Information about functions
 *  - Value flow analysis => possible values for each token
 *
 * Use --debug on the command line to see debug output for the token list
 * and the syntax tree. If both --debug and --verbose is used, the symbol
 * database is also written.
 *
 * The checks are written in C++. The checks are addons that can be
 * easily added/removed.
 *
 * @section writing_checks_sec Writing a check
 * Below is a simple example of a check that detect division with zero:
 * @code
void CheckOther::checkZeroDivision()
{
    // Iterate through all tokens in the token list
    for (const Token *tok = _tokenizer->tokens(); tok; tok = tok->next())
    {
        if (Token::Match(tok, "/ 0"))
            reportError(tok, Severity::error, "zerodiv", "Division by zero");
    }
}
 @endcode
 *
 * The function Token::Match is often used in the checks. Through it
 * you can match tokens against patterns. It is currently not possible
 * to write match expressions that uses the syntax tree, the symbol database,
 * nor the library. Only the token list is used.
 *
 * @section checkclass_sec Creating a new check class from scratch
 * %Check classes inherit from the Check class. The Check class specifies the interface that you must use.
 * To integrate a check class into clint all you need to do is:
 * - Add your source file(s) so they are compiled into the executable.
 * - Create an instance of the class (the Check::Check() constructor registers the class as an addon that Clint then can use).
 *
 *
 * @section embedding_sec Embedding Clint
 * Clint is designed to be easily embeddable into other programs.
 *
 * The "cli/main.cpp" and "cli/clintexecutor.*" files illustrate how clint
 * can be embedded into an application.
 *
 *
 * @section detailed_overview_sec Detailed overview
 * This happens when you execute clint from the command line:
 * -# CLintExecutor::check this function executes the Clint
 * -# Clint::parseFromArgs parse command line arguments
 *   - The Settings class is used to maintain settings
 *   - Use FileLister and command line arguments to get files to check
 * -# ThreadExecutor create more instances of Clint if needed
 * -# Clint::check is called for each file. It checks a single file
 * -# Preprocess the file (through Preprocessor)
 *   - Comments are removed
 *   - Macros are expanded
 * -# Tokenize the file (see Tokenizer)
 * -# Run the runChecks of all check classes.
 * -# Simplify the tokenlist (Tokenizer::simplifyTokenList)
 * -# Run the runSimplifiedChecks of all check classes
 *
 * When errors are found, they are reported back to the CLintExecutor through the ErrorLogger interface
 */


#include "clintexecutor.h"

#ifdef _WIN32
#include <windows.h>
static char exename[1024] = {0};
#endif

/**
 * Main function of clint
 *
 * @param argc Passed to Clint::parseFromArgs()
 * @param argv Passed to Clint::parseFromArgs()
 * @return What CLintExecutor::check() returns.
 */
int main(int argc, char* argv[])
{
    // MS Visual C++ memory leak debug tracing
#if defined(_MSC_VER) && defined(_DEBUG)
    _CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);
#endif

    CLintExecutor exec;
#ifdef _WIN32
    GetModuleFileNameA(NULL, exename, sizeof(exename)/sizeof(exename[0])-1);
    argv[0] = exename;
#endif
    return exec.check(argc, argv);
}


// Warn about deprecated compilers
#ifdef __clang__
#   if ( __clang_major__ < 2 || ( __clang_major__  == 2 && __clang_minor__ < 9))
#       warning "Using Clang 2.8 or earlier. Support for this version will be removed soon."
#   endif
#elif defined(__GNUC__)
#   if (__GNUC__ < 4 || (__GNUC__ == 4 && __GNUC_MINOR__ < 4))
#       warning "Using GCC 4.3 or earlier. Support for this version will be removed soon."
#   endif
#elif defined(_MSC_VER)
#   if (_MSC_VER < 1600)
#       pragma message("WARNING: Using Visual Studio 2008 or earlier. Support for this version will be removed soon.")
#   endif
#endif
