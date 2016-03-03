//---------------------------------------------------------------------------
#ifndef checkboostH
#define checkboostH
//---------------------------------------------------------------------------

#include "config.h"
#include "check.h"

class Token;

/// @addtogroup Checks
/// @{


/** @brief %Check Boost usage */
class CLINTLIB CheckBoost : public Check {
public:
    /** This constructor is used when registering the CheckClass */
    CheckBoost() : Check(myName()) {
    }

    /** This constructor is used when running checks. */
    CheckBoost(const Tokenizer *tokenizer, const Settings *settings, ErrorLogger *errorLogger)
        : Check(myName(), tokenizer, settings, errorLogger) {
    }

    /** Simplified checks. The token list is simplified. */
    void runSimplifiedChecks(const Tokenizer *tokenizer, const Settings *settings, ErrorLogger *errorLogger) {
        if (!tokenizer->isCPP())
            return;

        CheckBoost checkBoost(tokenizer, settings, errorLogger);

        checkBoost.checkBoostForeachModification();
    }

    /** @brief %Check for container modification while using the BOOST_FOREACH macro */
    void checkBoostForeachModification();

private:
    void boostForeachError(const Token *tok);

    void getErrorMessages(ErrorLogger *errorLogger, const Settings *settings) const {
        CheckBoost c(0, settings, errorLogger);
        c.boostForeachError(0);
    }

    static std::string myName() {
        return "Boost usage";
    }

    std::string classInfo() const {
        return "Check for invalid usage of Boost:\n"
               "* container modification during BOOST_FOREACH\n";
    }
};
/// @}
//---------------------------------------------------------------------------
#endif // checkboostH
