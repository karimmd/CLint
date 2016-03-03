//---------------------------------------------------------------------------
// 32-bit portability
//---------------------------------------------------------------------------

#include "check.h"

#include <iostream>

//---------------------------------------------------------------------------

Check::Check(const std::string &aname)
    : _tokenizer(0), _settings(0), _errorLogger(0), _name(aname)
{
    for (std::list<Check*>::iterator i = instances().begin(); i != instances().end(); ++i) {
        if ((*i)->name() > aname) {
            instances().insert(i, this);
            return;
        }
    }
    instances().push_back(this);
}

void Check::reportError(const ErrorLogger::ErrorMessage &errmsg)
{
    std::cout << errmsg.toXML(true, 1) << std::endl;
}
