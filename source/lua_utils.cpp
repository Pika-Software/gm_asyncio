#include "lua_utils.hpp"
#include <string>
#include <string_view>

namespace AsyncIO::LuaUtils {
    void FindValue(GarrysMod::Lua::ILuaBase* LUA, std::string_view path) {
        size_t firstPos = 0;
        size_t endPos = 0;
        do {
            firstPos = endPos;
            endPos = path.find(".", endPos) + 1;
            std::string name{ path.substr(firstPos, endPos != 0 ? endPos - firstPos - 1 : path.size()) };

            LUA->GetField(firstPos == 0 ? GarrysMod::Lua::INDEX_GLOBAL : -1, name.c_str());
            if (firstPos != 0) LUA->Remove(-2);
            if (!LUA->IsType(-1, GarrysMod::Lua::Type::Table)) break;
        } while (endPos != 0);
    }

    int RunFunction(GarrysMod::Lua::ILuaBase* LUA, int args, int results) {
        if (!LUA->IsType(-1 - args, GarrysMod::Lua::Type::Function)) {
            LUA->Pop(args + 1);
            for (int i = 0; i < results; i++) { LUA->PushNil(); }
            return 0;
        }
        LUA->GetField(GarrysMod::Lua::INDEX_GLOBAL, "ErrorNoHaltWithStack");
        LUA->Insert(-2 - args);
        if (LUA->PCall(args, results, -2 - args) != 0) { LUA->Pop(); };
        LUA->Remove(-1 - results);
        return 0;
    }
}