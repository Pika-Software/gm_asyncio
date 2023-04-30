# gm_asyncio [![Build][1]][2]
Make your Garry's Mod I/O asynchronous!

## Why?
Garry's Mod still doesn't have `file.AsyncWrite` function,.
Also `file.AsyncRead` also [doesn't work properly in menu state][3],
and Rubat doesn't want to elaborate in any way.

Also [gm_async_write](https://github.com/WilliamVenner/gm_async_write) exists, 
but it doesn't provide binaries for client-side and doesn't have binaries for MacOS.

## How to install?
1. Go to [Releases][4] page
2. Download binary that matches your OS and Garry's Mod branch
    * Select `gmsv_asyncio` binary if you need binary for your server
    or `gmcl_asyncio` if you need for clientside
    * Choose `asyncio_win`, `asyncio_linux` or `asyncio_osx` respectively to your OS
    * If you installed chromium or x86-64 Garry's Mod beta, then choose binary that ends with `win64.dll`/`linux64.dll`/`osx64.dll`, otherwise `win32.dll`/`linux.dll`/`osx.dll`
3. Place freshly downloaded binary to `<Garry's Mod>/garrysmod/lua/bin/` folder
4. Enjoy your smooth ride!

## For Developers
### API
```lua
-- Asynchronously writes given content to fileName in DATA folder
-- Have same limitations as https://wiki.facepunch.com/gmod/file.Write
-- Arguments:
--      fileName: path to a file
--      content: data to be written to a file
--      callback: function that will be called when async operation finishes
--                Arguments are:
--                    * fileName: string path to a file
--                    * gamePath: always "DATA"
--                    * status: see https://wiki.facepunch.com/gmod/Enums/FSASYNC
-- Returns:
--      status: FSASYNC_OK if successful, otherwise FSASYNC_ERR_
--              see https://wiki.facepunch.com/gmod/Enums/FSASYNC
number asyncio.AsyncWrite( string fileName, string content, function callback )

-- Same as asyncio.AsyncWrite, but instead of rewriting content of the file, just appends content to file
number asyncio.AsyncAppend( string fileName, string content, function callback )

-- Asynchronously reads content from a specified file
-- Have same limitations as https://wiki.facepunch.com/gmod/file.Read
-- Arguments:
--      fileName: path to a file
--      gamePath: see https://wiki.facepunch.com/gmod/File_Search_Paths
--      callback: function that will be called when async operation finishes
--                Arguments are:
--                    * fileName: string path to a file
--                    * gamePath: gamePath specified earlier
--                    * status: see https://wiki.facepunch.com/gmod/Enums/FSASYNC
--                    * data: contents of a file
-- Returns:
--      status: FSASYNC_OK if successful, otherwise FSASYNC_ERR_
--              see https://wiki.facepunch.com/gmod/Enums/FSASYNC
number asyncio.AsyncRead( string fileName, string gamePath, function callback )

-- Good luck with coding!
```

### Example
```lua
local data = "Hello World!"

local ok = asyncio.AsyncWrite("example.txt", data, function(fileName, gamePath, status)
    local ok = asyncio.AsyncRead(filename, gamePath, function(fileName, gamePath, status, fileContent)
        print(status, status == FSASYNC_OK) -- 0 true
        print(fileContent) -- Hello World!
        print(data == fileContent) -- true
    end)
end)

print("Write successful:", ok == FSASYNC_OK) -- Write successful: true
```

## Contributing
Feel free to create issues and pull request ❤️

[1]: https://github.com/Pika-Software/gm_asyncio/actions/workflows/build.yml/badge.svg?branch=main
[2]: https://github.com/Pika-Software/gm_asyncio/actions/workflows/build.yml
[3]: https://github.com/Facepunch/garrysmod-issues/issues/5433
[4]: https://github.com/Pika-Software/gm_asyncio/releases