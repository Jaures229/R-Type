/*
** EPITECH PROJECT, 2024
** B-CPP-500-COT-5-1-rtype-jaures.agossou
** File description:
** LuaScript
*/

#ifndef LUASCRIPT_HPP_
    #define LUASCRIPT_HPP_
    #include "sol/sol.hpp"
    #include <string>
    #include <iostream>

    class LuaScript {
        private:
            sol::state lua; // Instance de Lua

        public:
            LuaScript() {
                lua.open_libraries(sol::lib::base, sol::lib::math, sol::lib::table, sol::lib::os);
            }

            // Obtenir une fonction Lua et l'exécuter
            template <typename... Args>
            sol::object executeFunction(const std::string& funcName, Args... args) {
                sol::function func = lua[funcName];
                if (func.valid()) {
                    try {
                        return func(args...);
                    } catch (const sol::error& e) {
                        std::cerr << "Error executing function " << funcName << ": " << e.what() << std::endl;
                    }
                } else {
                    std::cerr << "Function " << funcName << " not found!" << std::endl;
                }
                return sol::nil;
            }

            // Obtenir une variable Lua
            template <typename T>
            T getVariable(const std::string& varName) {
                return lua[varName].get<T>();
            }

            // Passer une donnée C++ à Lua
            template <typename T>
            void setVariable(const std::string& varName, const T& value) {
                lua[varName] = value;
            }

            sol::state& getLuaState();

            bool loadScript(const std::string& scriptPath);

            sol::state& getState() {
                return lua; // Accède à l'état Lua
            }
        };

#endif /* !LUASCRIPT_HPP_ */
