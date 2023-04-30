#ifndef ASYNCIO_ASYNC_TASK
#define ASYNCIO_ASYNC_TASK

#include <filesystem.h>
#include <string>

namespace AsyncIO {
    enum AsyncTaskType {
        AsyncTaskType_Read,
        AsyncTaskType_Write,
    };

    struct AsyncTask {
        // Make this struct polymorphic
        virtual ~AsyncTask() = default;

        AsyncTaskType type;
        FSAsyncControl_t handle = nullptr;
        std::string fileName;
        std::string gamePath;
        int callbackRef = 0;
    };

    struct AsyncWriteTask : public AsyncTask {
        AsyncWriteTask(std::string_view fileName) {
            this->type = AsyncTaskType_Write;
            this->fileName = fileName;
            this->gamePath = "DATA";
        }

        int dataRef = 0;
    };

    struct AsyncReadTask : public AsyncTask {
        AsyncReadTask(std::string_view fileName, std::string_view gamePath) {
            this->type = AsyncTaskType_Read;
            this->fileName = fileName;
            this->gamePath = gamePath;
        }
    };
}

#endif // ASYNCIO_ASYNC_TASK