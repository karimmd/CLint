//---------------------------------------------------------------------------
// Obsolete functions
//---------------------------------------------------------------------------

#include "checkobsoletefunctions.h"
#include "symboldatabase.h"

//---------------------------------------------------------------------------


// Register this check class (by creating a static instance of it)
namespace {
    CheckObsoleteFunctions instance;
}

void CheckObsoleteFunctions::obsoleteFunctions()
{
    if (!_settings->isEnabled("style"))
        return;

    const SymbolDatabase *symbolDatabase = _tokenizer->getSymbolDatabase();

    // Functions defined somewhere?
    for (unsigned int i = 0; i < symbolDatabase->functionScopes.size(); i++) {
        const Scope* scope = symbolDatabase->functionScopes[i];
        _obsoleteStandardFunctions.erase(scope->className);
        _obsoletePosixFunctions.erase(scope->className);
        _obsoleteC99Functions.erase(scope->className);
    }

    for (unsigned int i = 0; i < symbolDatabase->functionScopes.size(); i++) {
        const Scope* scope = symbolDatabase->functionScopes[i];
        for (const Token* tok = scope->classStart; tok != scope->classEnd; tok = tok->next()) {
            if (tok->isName() && tok->varId()==0 && (tok->next() && tok->next()->str() == "(") &&
                (!Token::Match(tok->previous(), ".|::") || Token::simpleMatch(tok->tokAt(-2), "std ::"))) {

                std::map<std::string,std::string>::const_iterator it = _obsoleteStandardFunctions.find(tok->str());
                if (it != _obsoleteStandardFunctions.end()) {
                    // If checking an old code base it might be uninteresting to update obsolete functions.
                    reportError(tok, Severity::style, "obsoleteFunctions"+it->first, it->second);
                } else {
                    if (_settings->standards.posix) {
                        it = _obsoletePosixFunctions.find(tok->str());
                        if (it != _obsoletePosixFunctions.end()) {
                            // If checking an old code base it might be uninteresting to update obsolete functions.
                            reportError(tok, Severity::style, "obsoleteFunctions"+it->first, it->second);
                        }
                    }
                    if (_settings->standards.c >= Standards::C99) {
                        // alloca : this function is obsolete in C but not in C++ (#4382)
                        it = _obsoleteC99Functions.find(tok->str());
                        if (it != _obsoleteC99Functions.end() && !(tok->str() == "alloca" && _tokenizer->isCPP())) {
                            reportError(tok, Severity::style, "obsoleteFunctions"+it->first, it->second);
                        }
                    }
                }
            }
        }
    }
}
//---------------------------------------------------------------------------
