#include <path_utils.hpp>

#include <array>
#include <strtools.h>

namespace AsyncIO::PathUtils {
    const std::array RestrictedPathChars { ':', '"' };
    const std::array ExtensionWhitelist {
        "txt", "dat", "json", "xml", "csv",
        "jpg", "jpeg", "png", "vtf", "vmt",
        "mp3", "wav", "ogg", "vcd", "dem",
        "gma"
    };

    template<class T>
    inline bool Contains(std::string_view str, T value) {
        return str.find(value) != std::string_view::npos;
    }

    template<class T, std::size_t N>
    inline bool ContainsFromArray(std::string_view str, const std::array<T, N> values) {
        for (auto& v : values) {
            if (Contains(str, v))
                return true;
        }
        return false;
    }

    bool HasWhitelistedExtension(std::string_view fileName) {
        auto dotPos = fileName.rfind('.');
        if (dotPos == std::string_view::npos || dotPos == (fileName.size() - 1))
            return false;

        auto ext = fileName.substr(dotPos + 1);
        return ContainsFromArray(ext, ExtensionWhitelist);
    }

    bool FixPath(std::string& fileName) {
        if (ContainsFromArray(fileName, RestrictedPathChars))
            return false;

        // Lower-case path since unix file systems are case-sensitive
        V_strlower(&fileName[0]);

        // Fix backslashes to normal slashes
        V_FixSlashes(&fileName[0], '/');

        // Remove ./ and ../ from path, and return true if path valid (invalid if path goes past the root dir)
        return V_RemoveDotSlashes(&fileName[0], '/') && !V_IsAbsolutePath(fileName.c_str());
    }
}
