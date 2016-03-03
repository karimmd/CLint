//---------------------------------------------------------------------------
#ifndef check64bitH
#define check64bitH
//---------------------------------------------------------------------------

#include "config.h"
#include "check.h"


/// @addtogroup Checks
/// @{

/**
 * @brief Check for 64-bit portability issues
 */

class CLINTLIB Check64BitPortability : public Check {
public:
    /** This constructor is used when registering the Check64BitPortability */
    Check64BitPortability() : Check(myName()) {
    }

    /** This constructor is used when running checks. */
    Check64BitPortability(const Tokenizer *tokenizer, const Settings *settings, ErrorLogger *errorLogger)
        : Check(myName(), tokenizer, settings, errorLogger) {
    }

    /** @brief Run checks against the normal token list */
    void runChecks(const Tokenizer *tokenizer, const Settings *settings, ErrorLogger *errorLogger) {
        Check64BitPortability check64BitPortability(tokenizer, settings, errorLogger);
        check64BitPortability.pointerassignment();
    }

    /** @brief Run checks against the simplified token list */
    void runSimplifiedChecks(const Tokenizer *tokenizer, const Settings *settings, ErrorLogger *errorLogger) {
        (void)tokenizer;
        (void)settings;
        (void)errorLogger;
    }

    /** Check for pointer assignment */
    void pointerassignment();

private:

    void assignmentAddressToIntegerError(const Token *tok);
    void assignmentIntegerToAddressError(const Token *tok);
    void returnIntegerError(const Token *tok);
    void returnPointerError(const Token *tok);

    void getErrorMessages(ErrorLogger *errorLogger, const Settings *settings) const {
        Check64BitPortability c(0, settings, errorLogger);
        c.assignmentAddressToIntegerError(0);
        c.assignmentIntegerToAddressError(0);
        c.returnIntegerError(0);
        c.returnPointerError(0);
    }

    static std::string myName() {
        return "64-bit portability";
    }

    std::string classInfo() const {
        return "Check if there is 64-bit portability issues:\n"
               "* assign address to/from int/long\n"
               "* casting address from/to integer when returning from function\n";
    }
};
/// @}
//---------------------------------------------------------------------------
#endif // check64bitH
