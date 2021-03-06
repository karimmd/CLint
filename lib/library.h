//---------------------------------------------------------------------------
#ifndef libraryH
#define libraryH
//---------------------------------------------------------------------------

#include "config.h"
#include "path.h"
#include "mathlib.h"
#include "token.h"

#include <map>
#include <set>
#include <string>
#include <list>

class TokenList;
namespace tinyxml2 {
    class XMLDocument;
}

/// @addtogroup Core
/// @{

/**
 * @brief Library definitions handling
 */
class CLINTLIB Library {
public:
    Library();

    enum ErrorCode { OK, FILE_NOT_FOUND, BAD_XML, BAD_ELEMENT, MISSING_ATTRIBUTE, BAD_ATTRIBUTE, BAD_ATTRIBUTE_VALUE };

    class Error {
    public:
        Error() : errorcode(OK) , reason("") {}
        explicit Error(ErrorCode e) : errorcode(e) , reason("") {}
        Error(ErrorCode e, const std::string &r) : errorcode(e), reason(r) {}
        ErrorCode     errorcode;
        std::string   reason;
    };

    Error load(const char exename [], const char path []);
    Error load(const tinyxml2::XMLDocument &doc);

    /** this is primarily meant for unit tests. it only returns true/false */
    bool loadxmldata(const char xmldata[], std::size_t len);

    /** get allocation id for function by name */
    int alloc(const char name[]) const {
        return getid(_alloc, name);
    }

    /** get allocation id for function */
    int alloc(const Token *tok) const {
        return tok->function() ? 0 : getid(_alloc, tok->str());
    }

    /** get deallocation id for function */
    int dealloc(const Token *tok) const {
        return tok->function() ? 0 : getid(_dealloc, tok->str());
    }

    /** get deallocation id for function by name */
    int dealloc(const char name[]) const {
        return getid(_dealloc, name);
    }

    /** set allocation id for function */
    void setalloc(const std::string &functionname, int id) {
        _alloc[functionname] = id;
    }

    void setdealloc(const std::string &functionname, int id) {
        _dealloc[functionname] = id;
    }

    /** add noreturn function setting */
    void setnoreturn(const std::string& funcname, bool noreturn) {
        _noreturn[funcname] = noreturn;
    }

    /** is allocation type memory? */
    static bool ismemory(int id) {
        return ((id > 0) && ((id & 1) == 0));
    }

    /** is allocation type resource? */
    static bool isresource(int id) {
        return ((id > 0) && ((id & 1) == 1));
    }

    bool formatstr_function(const std::string& funcname) const {
        return _formatstr.find(funcname) != _formatstr.end();
    }

    bool formatstr_scan(const std::string& funcname) const {
        return _formatstr.at(funcname).first;
    }

    bool formatstr_secure(const std::string& funcname) const {
        return _formatstr.at(funcname).second;
    }

    std::set<std::string> use;
    std::set<std::string> leakignore;
    std::set<std::string> functionconst;
    std::set<std::string> functionpure;

    bool isnoreturn(const std::string &name) const {
        std::map<std::string, bool>::const_iterator it = _noreturn.find(name);
        return (it != _noreturn.end() && it->second);
    }

    bool isnotnoreturn(const std::string &name) const {
        std::map<std::string, bool>::const_iterator it = _noreturn.find(name);
        return (it != _noreturn.end() && !it->second);
    }

    bool isScopeNoReturn(const Token *end, std::string *unknownFunc) const;

    class ArgumentChecks {
    public:
        ArgumentChecks() :
            notbool(false),
            notnull(false),
            notuninit(false),
            formatstr(false),
            strz(false) {
        }

        bool         notbool;
        bool         notnull;
        bool         notuninit;
        bool         formatstr;
        bool         strz;
        std::string  valid;

        class MinSize {
        public:
            enum Type {NONE,STRLEN,ARGVALUE,SIZEOF,MUL};
            MinSize(Type t, int a) : type(t), arg(a), arg2(0) {}
            Type type;
            int arg;
            int arg2;
        };
        std::list<MinSize> minsizes;
    };

    // function name, argument nr => argument data
    std::map<std::string, std::map<int, ArgumentChecks> > argumentChecks;

    bool isboolargbad(const std::string &functionName, int argnr) const {
        const ArgumentChecks *arg = getarg(functionName, argnr);
        return arg && arg->notbool;
    }

    bool isnullargbad(const std::string &functionName, int argnr) const {
        const ArgumentChecks *arg = getarg(functionName, argnr);
        return arg && arg->notnull;
    }

    bool isuninitargbad(const std::string &functionName, int argnr) const {
        const ArgumentChecks *arg = getarg(functionName, argnr);
        return arg && arg->notuninit;
    }

    bool isargformatstr(const std::string &functionName, int argnr) const {
        const ArgumentChecks *arg = getarg(functionName, argnr);
        return arg && arg->formatstr;
    }

    bool isargstrz(const std::string &functionName, int argnr) const {
        const ArgumentChecks *arg = getarg(functionName, argnr);
        return arg && arg->strz;
    }

    bool isargvalid(const std::string &functionName, int argnr, const MathLib::bigint argvalue) const;

    const std::string& validarg(const std::string &functionName, int argnr) const {
        const ArgumentChecks *arg = getarg(functionName, argnr);
        return arg ? arg->valid : emptyString;
    }

    bool hasminsize(const std::string &functionName) const {
        std::map<std::string, std::map<int, ArgumentChecks> >::const_iterator it1;
        it1 = argumentChecks.find(functionName);
        if (it1 == argumentChecks.end())
            return false;
        std::map<int,ArgumentChecks>::const_iterator it2;
        for (it2 = it1->second.begin(); it2 != it1->second.end(); ++it2) {
            if (!it2->second.minsizes.empty())
                return true;
        }
        return false;
    }

    const std::list<ArgumentChecks::MinSize> *argminsizes(const std::string &functionName, int argnr) const {
        const ArgumentChecks *arg = getarg(functionName, argnr);
        return arg ? &arg->minsizes : nullptr;
    }

    bool markupFile(const std::string &path) const {
        return _markupExtensions.find(Path::getFilenameExtensionInLowerCase(path)) != _markupExtensions.end();
    }

    bool processMarkupAfterCode(const std::string &path) const {
        const std::map<std::string, bool>::const_iterator it = _processAfterCode.find(Path::getFilenameExtensionInLowerCase(path));
        return (it == _processAfterCode.end() || it->second);
    }

    const std::set<std::string> &markupExtensions() const {
        return _markupExtensions;
    }

    bool reportErrors(const std::string &path) const {
        const std::map<std::string, bool>::const_iterator it = _reporterrors.find(Path::getFilenameExtensionInLowerCase(path));
        return (it == _reporterrors.end() || it->second);
    }

    bool ignorefunction(const std::string &function) const {
        return (_ignorefunction.find(function) != _ignorefunction.end());
    }

    bool isexecutableblock(const std::string &file, const std::string &token) const {
        const std::map<std::string, CodeBlock>::const_iterator it = _executableblocks.find(Path::getFilenameExtensionInLowerCase(file));
        return (it != _executableblocks.end() && it->second.isBlock(token));
    }

    int blockstartoffset(const std::string &file) const {
        int offset = -1;
        const std::map<std::string, CodeBlock>::const_iterator map_it
            = _executableblocks.find(Path::getFilenameExtensionInLowerCase(file));

        if (map_it != _executableblocks.end()) {
            offset = map_it->second.offset();
        }
        return offset;
    }

    const std::string& blockstart(const std::string &file) const {
        const std::map<std::string, CodeBlock>::const_iterator map_it
            = _executableblocks.find(Path::getFilenameExtensionInLowerCase(file));

        if (map_it != _executableblocks.end()) {
            return map_it->second.start();
        }
        return emptyString;
    }

    const std::string& blockend(const std::string &file) const {
        const std::map<std::string, CodeBlock>::const_iterator map_it
            = _executableblocks.find(Path::getFilenameExtensionInLowerCase(file));

        if (map_it != _executableblocks.end()) {
            return map_it->second.end();
        }
        return emptyString;
    }

    bool iskeyword(const std::string &file, const std::string &keyword) const {
        const std::map<std::string, std::set<std::string> >::const_iterator it =
            _keywords.find(Path::getFilenameExtensionInLowerCase(file));
        return (it != _keywords.end() && it->second.count(keyword));
    }

    bool isexporter(const std::string &prefix) const {
        return _exporters.find(prefix) != _exporters.end();
    }

    bool isexportedprefix(const std::string &prefix, const std::string &token) const {
        const std::map<std::string, ExportedFunctions>::const_iterator it = _exporters.find(prefix);
        return (it != _exporters.end() && it->second.isPrefix(token));
    }

    bool isexportedsuffix(const std::string &prefix, const std::string &token) const {
        const std::map<std::string, ExportedFunctions>::const_iterator it = _exporters.find(prefix);
        return (it != _exporters.end() && it->second.isSuffix(token));
    }

    bool isimporter(const std::string& file, const std::string &importer) const {
        const std::map<std::string, std::set<std::string> >::const_iterator it =
            _importers.find(Path::getFilenameExtensionInLowerCase(file));
        return (it != _importers.end() && it->second.count(importer) > 0);
    }

    bool isreflection(const std::string &token) const {
        const std::map<std::string,int>::const_iterator it
            = _reflection.find(token);
        return it != _reflection.end();
    }

    int reflectionArgument(const std::string &token) const {
        int argIndex = -1;
        const std::map<std::string,int>::const_iterator it
            = _reflection.find(token);
        if (it != _reflection.end()) {
            argIndex = it->second;
        }
        return argIndex;
    }

    std::set<std::string> returnuninitdata;
    std::vector<std::string> defines; // to provide some library defines

    struct PodType {
        unsigned int   size;
        char           sign;
    };
    const struct PodType *podtype(const std::string &name) const {
        const std::map<std::string, struct PodType>::const_iterator it = podtypes.find(name);
        return (it != podtypes.end()) ? &(it->second) : nullptr;
    }

private:
    class ExportedFunctions {
    public:
        void addPrefix(const std::string& prefix) {
            _prefixes.insert(prefix);
        }
        void addSuffix(const std::string& suffix) {
            _suffixes.insert(suffix);
        }
        bool isPrefix(const std::string& prefix) const {
            return (_prefixes.find(prefix) != _prefixes.end());
        }
        bool isSuffix(const std::string& suffix) const {
            return (_suffixes.find(suffix) != _suffixes.end());
        }

    private:
        std::set<std::string> _prefixes;
        std::set<std::string> _suffixes;
    };
    class CodeBlock {
    public:
        CodeBlock() : _offset(0) {}

        void setStart(const std::string& s) {
            _start = s;
        }
        void setEnd(const std::string& e) {
            _end = e;
        }
        void setOffset(const int o) {
            _offset = o;
        }
        void addBlock(const std::string& blockName) {
            _blocks.insert(blockName);
        }
        const std::string& start() const {
            return _start;
        }
        const std::string& end() const {
            return _end;
        }
        int offset() const {
            return _offset;
        }
        bool isBlock(const std::string& blockName) const {
            return _blocks.find(blockName) != _blocks.end();
        }

    private:
        std::string _start;
        std::string _end;
        int _offset;
        std::set<std::string> _blocks;
    };
    int allocid;
    std::map<std::string, int> _alloc; // allocation functions
    std::map<std::string, int> _dealloc; // deallocation functions
    std::map<std::string, bool> _noreturn; // is function noreturn?
    std::set<std::string> _ignorefunction; // ignore functions/macros from a library (gtk, qt etc)
    std::map<std::string, bool> _reporterrors;
    std::map<std::string, bool> _processAfterCode;
    std::set<std::string> _markupExtensions; // file extensions of markup files
    std::map<std::string, std::set<std::string> > _keywords; // keywords for code in the library
    std::map<std::string, CodeBlock> _executableblocks; // keywords for blocks of executable code
    std::map<std::string, ExportedFunctions> _exporters; // keywords that export variables/functions to libraries (meta-code/macros)
    std::map<std::string, std::set<std::string> > _importers; // keywords that import variables/functions
    std::map<std::string,int> _reflection; // invocation of reflection
    std::map<std::string, std::pair<bool, bool> > _formatstr; // Parameters for format string checking
    std::map<std::string, struct PodType> podtypes; // pod types

    const ArgumentChecks * getarg(const std::string &functionName, int argnr) const;

    static int getid(const std::map<std::string,int> &data, const std::string &name) {
        const std::map<std::string,int>::const_iterator it = data.find(name);
        return (it == data.end()) ? 0 : it->second;
    }
};

/// @}
//---------------------------------------------------------------------------
#endif // libraryH
