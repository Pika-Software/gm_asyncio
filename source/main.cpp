#include "lua_utils.hpp"
#include "async_task.hpp"

#include <GarrysMod/Lua/Interface.h>
#include <GarrysMod/InterfacePointers.hpp>
#include <filesystem.h>

#include <memory>
#include <list>

IFileSystem* g_pFullFileSystem;

#undef AsyncRead

namespace AsyncIO {
    std::list<std::shared_ptr<AsyncTask>> Tasks;

    bool AddTask(std::shared_ptr<AsyncTask>&& task) {
        if (task->handle == nullptr)
            return false;

        Tasks.push_back(std::move(task));
    }

    bool FixFileName(std::string& fileName) {
        // TODO: Fix filename here
    }
}

namespace Lua {
    using namespace AsyncIO;

    LUA_FUNCTION(Think) {
        if (Tasks.empty())
            return 0;

        Tasks.remove_if([LUA](std::shared_ptr<AsyncTask>& task) {
            auto status = g_pFullFileSystem->AsyncStatus(task->handle);
            if (status == FSASYNC_STATUS_PENDING || status == FSASYNC_STATUS_INPROGRESS || status == FSASYNC_STATUS_UNSERVICED)
                return false;

            auto write_task = std::dynamic_pointer_cast<AsyncWriteTask>(task);
            auto read_task = std::dynamic_pointer_cast<AsyncReadTask>(task);

            std::string fileName = task->fileName.substr(5);

            if (task->callbackRef != 0) {
                LUA->ReferencePush(task->callbackRef);
                LUA->PushString(fileName.c_str());
                LUA->PushString(task->gamePath.c_str());
                if (write_task) {
                    LUA->PushNumber(status);
                    LuaUtils::RunFunction(LUA, 3, 0);
                } else if (read_task) {
                    void* data = nullptr;
                    int dataLen = 0;
                    if (status == FSASYNC_OK) {
                        status = g_pFullFileSystem->AsyncGetResult(read_task->handle, &data, &dataLen);
                    }

                    LUA->PushNumber(status);
                    LUA->PushString(reinterpret_cast<const char*>(data), dataLen);
                    LuaUtils::RunFunction(LUA, 4, 0);

                    MemAlloc_Free(data);
                }
                else {
                    LUA->PushNumber(FSASYNC_ERR_NOT_MINE);
                    LuaUtils::RunFunction(LUA, 3, 0);
                }
            }

            LUA->ReferenceFree(task->callbackRef);
            if (write_task)
                LUA->ReferenceFree(write_task->dataRef);

            g_pFullFileSystem->AsyncRelease(task->handle);

            return true;
        });

        return 0;
    }

    LUA_FUNCTION(AsyncWrite) {
        std::string fileName = LUA->CheckString(1);
        LUA->CheckString(2);

        // Fix fileName here
        fileName.insert(0, "data/");

        unsigned int contentLen;
        const char* content = LUA->GetString(2, &contentLen);

        auto task = std::make_shared<AsyncWriteTask>(fileName);
        auto status = g_pFullFileSystem->AsyncWrite(task->fileName.c_str(), content, contentLen, false, false, &task->handle);
        if (status == FSASYNC_OK) {
            LUA->Push(2);
            task->dataRef = LUA->ReferenceCreate();

            if (LUA->IsType(3, GarrysMod::Lua::Type::Function)) {
                LUA->Push(3);
                task->callbackRef = LUA->ReferenceCreate();
            }

            AddTask(std::move(task));
        }

        LUA->PushNumber(status);
        return 1;
    }

    LUA_FUNCTION(AsyncAppend) {
        std::string fileName = LUA->CheckString(1);
        LUA->CheckString(2);

        // Fix fileName here

        unsigned int contentLen;
        const char* content = LUA->GetString(2, &contentLen);

        auto task = std::make_shared<AsyncWriteTask>(fileName);

        LUA->Push(2);
        task->dataRef = LUA->ReferenceCreate();

        if (LUA->IsType(3, GarrysMod::Lua::Type::Function)) {
            LUA->Push(3);
            task->callbackRef = LUA->ReferenceCreate();
        }

        auto status = g_pFullFileSystem->AsyncAppend(fileName.c_str(), content, contentLen, false, &task->handle);
        if (status == FSASYNC_OK) {
            AddTask(std::move(task));
        }

        LUA->PushNumber(status);
        return 1;
    }

    LUA_FUNCTION(AsyncRead) {
        std::string fileName = LUA->CheckString(1);
        std::string gamePath = LUA->CheckString(2);

        // Fix fileName here

        auto task = std::make_shared<AsyncReadTask>(fileName, gamePath);
        if (LUA->IsType(3, GarrysMod::Lua::Type::Function)) {
            LUA->Push(3);
            task->callbackRef = LUA->ReferenceCreate();
        }

        FileAsyncRequest_t request{};
        request.pszFilename = task->fileName.c_str();
        request.pszPathID = task->gamePath.c_str();
        request.flags = FSASYNC_FLAGS_ALLOCNOFREE;

        auto status = g_pFullFileSystem->AsyncRead(request, &task->handle);
        if (status == FSASYNC_OK) {
            AddTask(std::move(task));
        }

        LUA->PushNumber(status);
        return 1;
    }
}

GMOD_MODULE_OPEN() {
    g_pFullFileSystem = InterfacePointers::FileSystem();
    if (!g_pFullFileSystem)
        LUA->ThrowError("failed to load source filesystem :(");

    // Initialize think hook
    AsyncIO::LuaUtils::FindValue(LUA, "timer.Create");
    LUA->PushString("__ASYNC_IO_THINK");
    LUA->PushNumber(0);
    LUA->PushNumber(0);
    LUA->PushCFunction(Lua::Think);
    AsyncIO::LuaUtils::RunFunction(LUA, 4, 0);

    LUA->GetField(GarrysMod::Lua::INDEX_GLOBAL, "async_io");
    if (!LUA->IsType(-1, GarrysMod::Lua::Type::Table)) {
        LUA->Pop();
        LUA->CreateTable();
    }
        LUA->PushCFunction(Lua::AsyncAppend);
        LUA->SetField(-2, "AsyncAppend");

        LUA->PushCFunction(Lua::AsyncWrite);
        LUA->SetField(-2, "AsyncWrite");

        LUA->PushCFunction(Lua::AsyncRead);
        LUA->SetField(-2, "AsyncRead");
    LUA->SetField(GarrysMod::Lua::INDEX_GLOBAL, "async_io");

    return 0;
}

GMOD_MODULE_CLOSE() {
    return 0;
}