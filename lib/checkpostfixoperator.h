//---------------------------------------------------------------------------
#ifndef checkpostfixoperatorH
#define checkpostfixoperatorH
//---------------------------------------------------------------------------

#include "config.h"
#include "check.h"

/// @addtogroup Checks
/// @{

/**
 * @brief Using postfix operators ++ or -- rather than postfix operator.
 */

class CLINTLIB CheckPostfixOperator : public Check {
public:
    /** This constructor is used when registering the CheckPostfixOperator */
    CheckPostfixOperator() : Check(myName()) {
    }

    /** This constructor is used when running checks. */
    CheckPostfixOperator(const Tokenizer *tokenizer, const Settings *settings, ErrorLogger *errorLogger)
        : Check(myName(), tokenizer, settings, errorLogger) {
    }

    void runSimplifiedChecks(const Tokenizer *tokenizer, const Settings *settings, ErrorLogger *errorLogger) {
        if (tokenizer->isC())
            return;

        CheckPostfixOperator checkPostfixOperator(tokenizer, settings, errorLogger);
        checkPostfixOperator.postfixOperator();
    }

    /** Check postfix operators */
    void postfixOperator();

private:
    /** Report Error */
    void postfixOperatorError(const Token *tok);

    void getErrorMessages(ErrorLogger *errorLogger, const Settings *settings) const {
        CheckPostfixOperator c(0, settings, errorLogger);
        c.postfixOperatorError(0);
    }

    static std::string myName() {
        return "Using postfix operators";
    }

    std::string classInfo() const {
        return "Warn if using postfix operators ++ or -- rather than prefix operator\n";
    }
};
/// @}
//---------------------------------------------------------------------------
#endif // checkpostfixoperatorH
