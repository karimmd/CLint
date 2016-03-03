#ifndef PATHMATCH_H
#define PATHMATCH_H

#include <string>
#include <vector>

/// @addtogroup CLI
/// @{

/**
 * @brief Simple path matching for ignoring paths in CLI.
 */
class PathMatch {
public:

    /**
     * The constructor.
     * @param masks List of masks.
     * @param caseSensitive Match the case of the characters when
     *   matching paths?
     */
    PathMatch(const std::vector<std::string> &masks, bool caseSensitive = true);

    /**
     * @brief Match path against list of masks.
     * @param path Path to match.
     * @return true if any of the masks match the path, false otherwise.
     */
    bool Match(const std::string &path) const;

protected:

    /**
     * @brief Remove filename part from the path.
     * @param path Path to edit.
     * @return path without filename part.
     */
    static std::string RemoveFilename(const std::string &path);

private:
    std::vector<std::string> _masks;
    bool _caseSensitive;
};

/// @}

#endif // PATHMATCH_H
