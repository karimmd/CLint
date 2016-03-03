//---------------------------------------------------------------------------
#ifndef checkautovariablesH
#define checkautovariablesH
//---------------------------------------------------------------------------

#include "config.h"
#include "check.h"

/// @addtogroup Checks
/** @brief Various small checks for automatic variables */
/// @{


class CLINTLIB CheckAutoVariables : public Check {
public:
    /** This constructor is used when registering the CheckClass */
    CheckAutoVariables() : Check(myName()) {
    }

    /** This constructor is used when running checks. */
    CheckAutoVariables(const Tokenizer *tokenizer, const Settings *settings, ErrorLogger *errorLogger)
        : Check(myName(), tokenizer, settings, errorLogger) {
    }

    /** @brief Run checks against the normal token list */
    void runChecks(const Tokenizer *tokenizer, const Settings *settings, ErrorLogger *errorLogger) {
        CheckAutoVariables checkAutoVariables(tokenizer, settings, errorLogger);
        checkAutoVariables.assignFunctionArg();
        checkAutoVariables.returnReference();
    }

    void runSimplifiedChecks(const Tokenizer *tokenizer, const Settings *settings, ErrorLogger *errorLogger) {
        CheckAutoVariables checkAutoVariables(tokenizer, settings, errorLogger);
        checkAutoVariables.autoVariables();
        checkAutoVariables.returnPointerToLocalArray();
    }

    /** assign function argument */
    void assignFunctionArg();

    /** Check auto variables */
    void autoVariables();

    /** Returning pointer to local array */
    void returnPointerToLocalArray();

    /** Returning reference to local/temporary variable */
    void returnReference();

private:
    static bool isPtrArg(const Token *tok);
    static bool isRefPtrArg(const Token *tok);
    static bool isNonReferenceArg(const Token *tok);
    static bool isAutoVar(const Token *tok);
    static bool isAutoVarArray(const Token *tok);

    /**
     * Returning a temporary object?
     * @param tok pointing at the "return" token
     * @return true if a temporary object is returned
     */
    bool returnTemporary(const Token *tok) const;

    void errorReturnAddressToAutoVariable(const Token *tok);
    void errorReturnPointerToLocalArray(const Token *tok);
    void errorAutoVariableAssignment(const Token *tok, bool inconclusive);
    void errorReturnReference(const Token *tok);
    void errorReturnTempReference(const Token *tok);
    void errorInvalidDeallocation(const Token *tok);
    void errorReturnAddressOfFunctionParameter(const Token *tok, const std::string &varname);
    void errorUselessAssignmentPtrArg(const Token *tok);

    void getErrorMessages(ErrorLogger *errorLogger, const Settings *settings) const {
        CheckAutoVariables c(0,settings,errorLogger);
        c.errorAutoVariableAssignment(0, false);
        c.errorReturnAddressToAutoVariable(0);
        c.errorReturnPointerToLocalArray(0);
        c.errorReturnReference(0);
        c.errorReturnTempReference(0);
        c.errorInvalidDeallocation(0);
        c.errorReturnAddressOfFunctionParameter(0, "parameter");
        c.errorUselessAssignmentPtrArg(0);
    }

    static std::string myName() {
        return "Auto Variables";
    }

    std::string classInfo() const {
        return "A pointer to a variable is only valid as long as the variable is in scope.\n"
               "Check:\n"
               "* returning a pointer to auto or temporary variable\n"
               "* assigning address of an variable to an effective parameter of a function\n"
               "* returning reference to local/temporary variable\n"
               "* returning address of function parameter\n"
               "* useless assignment of pointer parameter\n";
    }
};
/// @}
//---------------------------------------------------------------------------
#endif // checkautovariablesH
