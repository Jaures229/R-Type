/*
** EPITECH PROJECT, 2024
** B-CPP-500-COT-5-1-rtype-jaures.agossou
** File description:
** LuaScript
*/

#include "../Include/LuaScript.hpp"


// Charger un script Lua
bool LuaScript::loadScript(const std::string& scriptPath)
{
    try {
        lua.script_file(scriptPath);
        return true;
    } catch (const sol::error& e) {
        std::cerr << "Error loading Lua script: " << e.what() << std::endl;
        return false;
    }
}


sol::state& LuaScript::getLuaState() { return lua; }
