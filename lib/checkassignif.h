//---------------------------------------------------------------------------
#ifndef checkassignifH
#define checkassignifH
//---------------------------------------------------------------------------

#include "config.h"
#include "check.h"
#include "mathlib.h"

/// @addtogroup Checks
/// @{

/**
 * @brief Check for assignment / condition mismatches
 */

class CLINTLIB CheckAssignIf : public Check {
public:
    /** This constructor is used when registering the CheckAssignIf */
    CheckAssignIf() : Check(myName()) {
    }

    /** This constructor is used when running checks. */
    CheckAssignIf(const Tokenizer *tokenizer, const Settings *settings, ErrorLogger *errorLogger)
        : Check(myName(), tokenizer, settings, errorLogger) {
    }

    void runChecks(const Tokenizer *tokenizer, const Settings *settings, ErrorLogger *errorLogger) {
        CheckAssignIf checkAssignIf(tokenizer, settings, errorLogger);
        checkAssignIf.multiCondition();
    }

    /** @brief Run checks against the simplified token list */
    void runSimplifiedChecks(const Tokenizer *tokenizer, const Settings *settings, ErrorLogger *errorLogger) {
        CheckAssignIf checkAssignIf(tokenizer, settings, errorLogger);
        checkAssignIf.assignIf();
        checkAssignIf.comparison();
    }

    /** mismatching assignment / comparison */
    void assignIf();

    /** parse scopes recursively */
    bool assignIfParseScope(const Token * const assignTok,
                            const Token * const startTok,
                            const unsigned int varid,
                            const bool islocal,
                            const char bitop,
                            const MathLib::bigint num);

    /** mismatching lhs and rhs in comparison */
    void comparison();

    /** match 'if' and 'else if' conditions */
    void multiCondition();

private:

    void assignIfError(const Token *tok1, const Token *tok2, const std::string &condition, bool result);

    void mismatchingBitAndError(const Token *tok1, const MathLib::bigint num1, const Token *tok2, const MathLib::bigint num2);

    void comparisonError(const Token *tok,
                         const std::string &bitop,
                         MathLib::bigint value1,
                         const std::string &op,
                         MathLib::bigint value2,
                         bool result);

    void multiConditionError(const Token *tok, unsigned int line1);

    void getErrorMessages(ErrorLogger *errorLogger, const Settings *settings) const {
        CheckAssignIf c(0, settings, errorLogger);
        c.assignIfError(0, 0, "", false);
        c.comparisonError(0, "&", 6, "==", 1, false);
        c.multiConditionError(0,1);
        c.mismatchingBitAndError(0,0xf0, 0, 1);
    }

    static std::string myName() {
        return "AssignIf";
    }

    std::string classInfo() const {
        return "Match assignments and conditions:\n"
               "* Mismatching assignment and comparison => comparison is always true/false\n"
               "* Mismatching lhs and rhs in comparison => comparison is always true/false\n"
               "* Detect matching 'if' and 'else if' conditions\n"
               "* Mismatching bitand (a &= 0xf0; a &= 1; => a = 0)\n";
    }
};
/// @}
//---------------------------------------------------------------------------
#endif // checkassignifH
