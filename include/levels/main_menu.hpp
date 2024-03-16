// levels/main_menu.hpp
#pragma once

#include <levels/general.hpp>
#include <SDL.h>
#include <lua.hpp>

class MainMenu : public Level {
public:
    MainMenu();
    void setup() override;
    void show() override;
    void update(SDL_Event key) override;
    ~MainMenu();
private:
    lua_State* L;
    const char* luaScriptName = "levels/mainmenu.lua";

    // Declare C functions for OpenGL calls
    static int lua_glUseProgram(lua_State* L);
    static int lua_glDrawArrays(lua_State* L);
    static int lua_glBindTexture(lua_State* L);
    // Add other OpenGL function declarations here
};

