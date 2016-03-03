//---------------------------------------------------------------------------
#ifndef checkassertH
#define checkassertH
//---------------------------------------------------------------------------

#include "config.h"
#include "check.h"

/// @addtogroup Checks
/// @{

/**
 * @brief Checking for side effects in assert statements
 */

class CLINTLIB CheckAssert : public Check {
public:
    CheckAssert() : Check(myName()) {
    }

    CheckAssert(const Tokenizer *tokenizer, const Settings *settings, ErrorLogger *errorLogger)
        : Check(myName(), tokenizer, settings, errorLogger) {
    }

    virtual void runSimplifiedChecks(const Tokenizer *tokenizer, const Settings *settings, ErrorLogger *errorLogger) {
        CheckAssert check(tokenizer, settings, errorLogger);
        check.assertWithSideEffects();
    }

    void assertWithSideEffects();

protected:
    bool checkVariableAssignment(const Token* tmp, bool reportErr = true);
    static bool inSameScope(const Token* returnTok, const Token* assignTok);

    static const Token* findAssertPattern(const Token *start);

private:
    void sideEffectInAssertError(const Token *tok, const std::string& functionName);
    void assignmentInAssertError(const Token *tok, const std::string &varname);

    void getErrorMessages(ErrorLogger *errorLogger, const Settings *settings) const {
        CheckAssert c(0, settings, errorLogger);
        c.sideEffectInAssertError(0, "function");
        c.assignmentInAssertError(0, "var");
    }

    static std::string myName() {
        return "Assert";
    }

    std::string classInfo() const {
        return "Warn if there are side effects in assert statements (since this cause different behaviour in debug/release builds).\n";
    }
};
/// @}
//---------------------------------------------------------------------------
#endif // checkassertH
