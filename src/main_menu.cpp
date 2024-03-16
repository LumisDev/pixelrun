// levels/main_menu.cpp
#include <levels/main_menu.hpp>
#include <glad/glad.h>

MainMenu::MainMenu() {
    L = luaL_newstate();
    luaL_openlibs(L);
}

MainMenu::~MainMenu() {
    lua_close(L);
}

void MainMenu::setup() {
    luaL_dofile(L, luaScriptName);
    // Register OpenGL functions for Lua
    lua_register(L, "glUseProgram", lua_glUseProgram);
    lua_register(L, "glDrawArrays", lua_glDrawArrays);
    lua_register(L, "glBindTexture", lua_glBindTexture);
    // Register other OpenGL functions similarly
}

void MainMenu::show() {
    // Optionally, you can update the Lua variables here if needed
}

void MainMenu::update(SDL_Event key) {
    // Optionally, you can update the Lua variables here if needed
}

// Implement the C functions for each OpenGL function
int MainMenu::lua_glUseProgram(lua_State* L) {
    GLuint program = lua_tonumber(L, 1);
    glUseProgram(program);
    return 0;
}

int MainMenu::lua_glDrawArrays(lua_State* L) {
    GLenum mode = lua_tonumber(L, 1);
    GLint first = lua_tonumber(L, 2);
    GLsizei count = lua_tonumber(L, 3);
    glDrawArrays(mode, first, count);
    return 0;
}

int MainMenu::lua_glBindTexture(lua_State* L) {
    GLenum target = lua_tonumber(L, 1);
    GLuint texture = lua_tonumber(L, 2);
    glBindTexture(target, texture);
    return 0;
}
// Implement other OpenGL functions similarly
