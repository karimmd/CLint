//---------------------------------------------------------------------------
#ifndef checkleakautovarH
#define checkleakautovarH
//---------------------------------------------------------------------------

#include "config.h"
#include "check.h"

#include <map>
#include <set>


class CLINTLIB VarInfo {
public:
    std::map<unsigned int, int> alloctype;
    std::map<unsigned int, std::string> possibleUsage;
    std::set<unsigned int> conditionalAlloc;
    std::set<unsigned int> referenced;

    void clear() {
        alloctype.clear();
        possibleUsage.clear();
        conditionalAlloc.clear();
        referenced.clear();
    }

    void erase(unsigned int varid) {
        alloctype.erase(varid);
        possibleUsage.erase(varid);
        conditionalAlloc.erase(varid);
    }

    void swap(VarInfo &other) {
        alloctype.swap(other.alloctype);
        possibleUsage.swap(other.possibleUsage);
        conditionalAlloc.swap(other.conditionalAlloc);
        referenced.swap(other.referenced);
    }

    /** set possible usage for all variables */
    void possibleUsageAll(const std::string &functionName);

    void print();
};


/// @addtogroup Checks
/// @{

/**
 * @brief Check for leaks
 */

class CLINTLIB CheckLeakAutoVar : public Check {
public:
    /** This constructor is used when registering the CheckLeakAutoVar */
    CheckLeakAutoVar() : Check(myName()) {
    }

    /** This constructor is used when running checks. */
    CheckLeakAutoVar(const Tokenizer *tokenizer, const Settings *settings, ErrorLogger *errorLogger)
        : Check(myName(), tokenizer, settings, errorLogger) {
    }

    /** @brief Run checks against the simplified token list */
    void runSimplifiedChecks(const Tokenizer *tokenizer, const Settings *settings, ErrorLogger *errorLogger) {
        CheckLeakAutoVar checkLeakAutoVar(tokenizer, settings, errorLogger);
        checkLeakAutoVar.check();
    }

private:

    /** check for leaks in all scopes */
    void check();

    /** check for leaks in a function scope */
    void checkScope(const Token * const startToken,
                    VarInfo *varInfo,
                    std::set<unsigned int> notzero);

    /** parse function call */
    void functionCall(const Token *tok, VarInfo *varInfo, const int dealloc);

    /** return. either "return" or end of variable scope is seen */
    void ret(const Token *tok, const VarInfo &varInfo);

    /** if variable is allocated then there is a leak */
    void leakIfAllocated(const Token *vartok, const VarInfo &varInfo);

    void leakError(const Token* tok, const std::string &varname, int type);
    void mismatchError(const Token* tok, const std::string &varname);
    void deallocUseError(const Token *tok, const std::string &varname);
    void deallocReturnError(const Token *tok, const std::string &varname);

    /** message: user configuration is needed to complete analysis */
    void configurationInfo(const Token* tok, const std::string &functionName);

    void getErrorMessages(ErrorLogger *errorLogger, const Settings *settings) const {
        CheckLeakAutoVar c(0, settings, errorLogger);
        c.deallocReturnError(0, "p");
        c.configurationInfo(0, "f");  // user configuration is needed to complete analysis
    }

    static std::string myName() {
        return "Leaks (auto variables)";
    }

    std::string classInfo() const {
        return "Detect when a auto variable is allocated but not deallocated.\n";
    }
};
/// @}
//---------------------------------------------------------------------------
#endif // checkleakautovarH
