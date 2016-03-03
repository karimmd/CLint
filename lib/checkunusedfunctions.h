//---------------------------------------------------------------------------
#ifndef checkunusedfunctionsH
#define checkunusedfunctionsH
//---------------------------------------------------------------------------

#include "config.h"
#include "check.h"

/// @addtogroup Checks
/** @brief Check for functions never called */
/// @{

class CLINTLIB CheckUnusedFunctions : public Check {
public:
    /** @brief This constructor is used when registering the CheckUnusedFunctions */
    CheckUnusedFunctions() : Check(myName()) {
    }

    /** @brief This constructor is used when running checks. */
    CheckUnusedFunctions(const Tokenizer *tokenizer, const Settings *settings, ErrorLogger *errorLogger)
        : Check(myName(), tokenizer, settings, errorLogger) {
    }

    // Parse current tokens and determine..
    // * Check what functions are used
    // * What functions are declared
    void parseTokens(const Tokenizer &tokenizer, const char FileName[], const Settings *settings);

    void check(ErrorLogger * const errorLogger);

    static CheckUnusedFunctions instance;

private:

    void getErrorMessages(ErrorLogger *errorLogger, const Settings *settings) const {
        CheckUnusedFunctions c(0, settings, errorLogger);
        c.unusedFunctionError(errorLogger, "", 0, "funcName");
    }

    /**
     * Dummy implementation, just to provide error for --errorlist
     */
    static void unusedFunctionError(ErrorLogger * const errorLogger,
                                    const std::string &filename, unsigned int lineNumber,
                                    const std::string &funcname);

    /**
     * Dummy implementation, just to provide error for --errorlist
     */
    void runSimplifiedChecks(const Tokenizer *, const Settings *, ErrorLogger *) {

    }

    static std::string myName() {
        return "Unused functions";
    }

    std::string classInfo() const {
        return "Check for functions that are never called\n";
    }

    class CLINTLIB FunctionUsage {
    public:
        FunctionUsage() : lineNumber(0), usedSameFile(false), usedOtherFile(false) {
        }

        std::string filename;
        unsigned int lineNumber;
        bool   usedSameFile;
        bool   usedOtherFile;
    };

    std::map<std::string, FunctionUsage> _functions;
};
/// @}
//---------------------------------------------------------------------------
#endif // checkunusedfunctionsH
