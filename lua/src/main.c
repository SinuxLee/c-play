#include <stdio.h>
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"

int main()
{
    // 创建 Lua VM
    lua_State *L = luaL_newstate();
    // 打开标准库
    luaL_openlibs(L);

    // 载入脚本
    if (luaL_dofile(L, "lua/script/test.lua"))
    {
        printf("Lua error: %s\n", lua_tostring(L, -1));
        return 1;
    }

     // 获取 lua 函数
    lua_getglobal(L, "add");
    lua_pushinteger(L, 10);
    lua_pushinteger(L, 32);

    // 调用：2 参数，1 返回值
    if (lua_pcall(L, 2, 1, 0) != 0)
    {
        printf("Lua error: %s\n", lua_tostring(L, -1));
        return 1;
    }

    int result = lua_tointeger(L, -1);
    printf("result = %d\n", result);

    lua_close(L);
    return 0;
}
