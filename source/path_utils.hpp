#ifndef ASYNCIO_PATH_UTILS
#define ASYNCIO_PATH_UTILS

#include <string>
#include <string_view>

namespace AsyncIO::PathUtils {
    // Returns false if extension isn't whitelisted
    // Also returns false if extension is absent
    bool HasWhitelistedExtension(std::string_view fileName);

    // Turns path into lower-case, removes double slashes and "./" or "../".
    // If path escapes relative root directory, then returns false
    // If path is absolute, then also returns false
    // Otherwise returns true
    bool FixPath(std::string& fileName);
}


#endif // ASYNCIO_PATH_UTILS