//---------------------------------------------------------------------------
#ifndef valueflowH
#define valueflowH
//---------------------------------------------------------------------------

class Token;
class TokenList;
class ErrorLogger;
class Settings;

namespace ValueFlow {
    class Value {
    public:
        Value(long long val = 0) : intvalue(val), varvalue(val), condition(0), varId(0U), conditional(false), inconclusive(false) {}
        Value(const Token *c, long long val) : intvalue(val), varvalue(val), condition(c), varId(0U), conditional(false), inconclusive(false) {}

        /** int value */
        long long intvalue;

        /** For calculated values - variable value that calculated value depends on */
        long long varvalue;

        /** Condition that this value depends on (TODO: replace with a 'callstack') */
        const Token *condition;

        /** For calculated values - varId that calculated value depends on */
        unsigned int varId;

        /** Conditional value */
        bool conditional;

        /** Is this value inconclusive? */
        bool inconclusive;
    };

    void setValues(TokenList *tokenlist, ErrorLogger *errorLogger, const Settings *settings);
}

#endif // valueflowH
