#include <stdlib.h>
#include <stdio.h>

extern "C"
{
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}




// g++ -L/usr/local/lib -llua -I/usr/local/include -Wall -o c2lua.out


int testfunc();

int testfunc2();

int testfunc()
{
    lua_State * L = luaL_newstate();
    luaL_openlibs(L);

    if(0 != luaL_dofile(L, "./scripts/lua1.lua"))
    {
        printf("luaL_dofile %s\n", lua_tostring(L, -1));
        return 0;
    }

    lua_getglobal(L, "showinfo");

    if(0 != lua_pcall(L, 0, 0, 0))
    {
        printf("lua_pcall [%s]\n", lua_tostring(L, -1));
        return 0;
    }

    const char* ptr = "main message from cplusplus!  \n";
    lua_getglobal(L,"showstr");
    lua_pushstring(L, ptr);
    if (0 != lua_pcall(L,1,0,0))
    {
        printf("%s\n", lua_tostring(L, -1));
        return 0;
    }

    lua_getglobal(L,"add");
    lua_pushinteger(L,2);  
    lua_pushinteger(L,3);
    if (0 != lua_pcall(L,2,1,0))
    {
        printf("lua_pcall [%s]\n", lua_tostring(L, -1));
        return 0;
    }

    printf("lua add function return val is %lld\n", lua_tointeger(L,-1));

    lua_close(L);
    return 0;
}


void init_lua(lua_State* L)  
{  
    luaL_openlibs(L);  
    luaopen_base(L);  
    luaopen_table(L);  
    luaopen_string(L);  
    luaopen_math(L);  
}  

int c_add(lua_State* L)  
{  
    int a = lua_tonumber(L, -2);  
    int b = lua_tonumber(L, -1);  
    int c = a + b;  
    lua_pushnumber(L, c);  
    return 1;  
}  

int c_step(lua_State* L)  
{  
    int a = lua_tonumber(L, -1);  
    int c = a + 1;  
    lua_pushnumber(L, c);  
    return 1;  
}  

int testfunc2()
{
    printf("\ntestfunc2 =======\n");
    lua_State *L = luaL_newstate();  
    init_lua(L);  
  
    if (luaL_loadfile(L, "./scripts/lua2.lua") != 0) {  
        printf("fail to load\n");  
        return 0;  
    }

    //everything in lua is variable (including functions), so we need to init them.  
    if (lua_pcall(L, 0, 0, 0) != 0) {  
        printf("fail to run lua: %s\n", lua_tostring(L, -1));  
        return 0;  
    }

    //prepare additional functions for lua to call  
    lua_register(L, "c_add", c_add);
    lua_register(L, "c_step", c_step);
      
    //c call lua  
    lua_getglobal(L, "l_ff");  
    lua_pushnumber(L, 2);  
    lua_pushnumber(L, 3);  
    if (lua_pcall(L, 2, 1, 0) != 0) {  
        printf("fail to call func: %s\n", lua_tostring(L, -1));  
        return 0;  
    }  


    return 0;
}


int main(int argc,char ** argv)
{

    testfunc();

    testfunc2();

    return 1;
}
