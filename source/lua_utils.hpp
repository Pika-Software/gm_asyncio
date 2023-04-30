#ifndef ASYNCIO_LUAQUICKS
#define ASYNCIO_LUAQUICKS

#include <GarrysMod/Lua/LuaBase.h>
#include <string_view>

namespace AsyncIO::LuaUtils {
    // path = "atmosphere.table.value"
    // Pushes to the stack `value` from `atmosphere.table`
    void FindValue(GarrysMod::Lua::ILuaBase* LUA, std::string_view path);

    // Runs a function with given arguments on the stack. If function is nil, then just pushes nils as results and pops everything else
    int RunFunction(GarrysMod::Lua::ILuaBase* LUA, int args, int results);
}

#endif // ASYNCIO_LUAQUICKS
