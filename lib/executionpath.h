//---------------------------------------------------------------------------
#ifndef executionpathH
#define executionpathH
//---------------------------------------------------------------------------

#include <list>
#include "config.h"

class Token;
class Check;
class SymbolDatabase;

/**
 * Base class for Execution Paths checking
 * An execution path is a linear list of statements. There are no "if"/.. to worry about.
 **/
class CLINTLIB ExecutionPath {
private:
    /** No implementation */
    void operator=(const ExecutionPath &);

protected:
    Check * const owner;

    /** Are two execution paths equal? */
    virtual bool is_equal(const ExecutionPath *) const = 0;

public:
    ExecutionPath(Check *c, unsigned int id) : owner(c), numberOfIf(0), varId(id) {
    }

    virtual ~ExecutionPath() {
    }

    /** Implement this in each derived class. This function must create a copy of the current instance */
    virtual ExecutionPath *copy() = 0;

    /** print checkdata */
    void print() const;

    /** number of if blocks */
    unsigned int numberOfIf;

    const unsigned int varId;

    /**
     * bail out all execution paths
     * @param checks the execution paths to bail out on
     **/
    static void bailOut(std::list<ExecutionPath *> &checks) {
        while (!checks.empty()) {
            delete checks.back();
            checks.pop_back();
        }
    }

    /**
     * bail out execution paths with given variable id
     * @param checks the execution paths to bail out on
     * @param varid the specific variable id
     **/
    static void bailOutVar(std::list<ExecutionPath *> &checks, const unsigned int varid) {
        if (varid == 0)
            return;

        std::list<ExecutionPath *>::iterator it = checks.begin();
        while (it != checks.end()) {
            if ((*it)->varId == varid) {
                delete *it;
                checks.erase(it++);
            } else {
                ++it;
            }
        }
    }

    /**
     * Parse tokens at given location
     * @param tok token to parse
     * @param checks The execution paths. All execution paths in the list are executed in the current scope.
     * @return the token before the "next" token.
     **/
    virtual const Token *parse(const Token &tok, std::list<ExecutionPath *> &checks) const = 0;

    /**
     * Parse condition
     * @param tok first token in condition.
     * @param checks The execution paths. All execution paths in the list are executed in the current scope
     * @return true => bail out all checking
     **/
    virtual bool parseCondition(const Token &tok, std::list<ExecutionPath *> &checks);

    /**
     * Parse loop body
     * @param tok the first token in the loop body (the token after the {)
     * @param checks The execution paths
     */
    virtual void parseLoopBody(const Token *tok, std::list<ExecutionPath *> &checks) const {
        (void)tok;
        (void)checks;
    }

    /** going out of scope - all execution paths end */
    virtual void end(const std::list<ExecutionPath *> & /*checks*/, const Token * /*tok*/) const {
    }

    bool operator==(const ExecutionPath &e) const {
        return bool(varId == e.varId && is_equal(&e));
    }

    static void checkScope(const Token *tok, std::list<ExecutionPath *> &checks);
};


void checkExecutionPaths(const SymbolDatabase *symbolDatabase, ExecutionPath *c);

//---------------------------------------------------------------------------
#endif // executionpathH
