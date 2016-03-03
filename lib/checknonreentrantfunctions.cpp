//---------------------------------------------------------------------------
// Find non reentrant functions
//---------------------------------------------------------------------------

#include "checknonreentrantfunctions.h"

//---------------------------------------------------------------------------


// Register this check class (by creating a static instance of it)
namespace {
    CheckNonReentrantFunctions instance;
}

void CheckNonReentrantFunctions::nonReentrantFunctions()
{
    if (!_settings->standards.posix || !_settings->isEnabled("portability"))
        return;

    std::map<std::string,std::string>::const_iterator nonReentrant_end = _nonReentrantFunctions.end();
    for (const Token *tok = _tokenizer->tokens(); tok; tok = tok->next()) {
        // Look for function invocations
        if (!tok->isName() || tok->strAt(1) != "(" || tok->varId() != 0)
            continue;

        // Check for non-reentrant function name
        std::map<std::string,std::string>::const_iterator it = _nonReentrantFunctions.find(tok->str());
        if (it == nonReentrant_end)
            continue;

        const Token *prev = tok->previous();
        if (prev) {
            // Ignore function definitions, class members or class definitions
            if (prev->isName() || Token::Match(prev, ".|:"))
                continue;

            // Check for "std" or global namespace, ignore other namespaces
            if (prev->str() == "::" && prev->previous() && prev->previous()->str() != "std" && prev->previous()->isName())
                continue;
        }

        // Only affecting multi threaded code, therefore this is "portability"
        reportError(tok, Severity::portability, "nonreentrantFunctions"+it->first, it->second);
    }
}
//---------------------------------------------------------------------------
