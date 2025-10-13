@echo off
setlocal enabledelayedexpansion

:: Vérification des Outils
where conan >nul 2>&1 || (
    echo Conan n'est pas installé. Installez avec 'pip install conan'.
    exit /b 1
)

where cmake >nul 2>&1 || (
    echo CMake n'est pas installé.
    exit /b 1
)

:: Log des Opérations
echo [INFO] Début du processus de build

:: Profil Conan
echo [CONAN] Détection du profil système
conan profile detect --force --name default
if !errorlevel! neq 0 (
    echo [ERREUR] Échec de la détection du profil Conan
    exit /b 1
)

:: Installation des Dépendances
echo [CONAN] Installation des dépendances
conan install . -s compiler.cppstd=17 -s build_type=Release --build=missing
if !errorlevel! neq 0 (
    echo [ERREUR] Échec de l'installation des dépendances
    exit /b 1
)

:: Configuration CMake avec C++17
echo [CMAKE] Configuration du projet avec C++17
cmake -G "Visual Studio 17 2022" ^
    -DCMAKE_CXX_STANDARD=17 ^
    -DCMAKE_CXX_STANDARD_REQUIRED=ON ^
    -DCMAKE_CXX_EXTENSIONS=OFF ^
    -DCMAKE_BUILD_TYPE=Release .
if !errorlevel! neq 0 (
    echo [ERREUR] Échec de la configuration CMake
    exit /b 1
)

:: Construction du Projet
echo [CMAKE] Construction du projet
cmake --build . --config Release
if !errorlevel! neq 0 (
    echo [ERREUR] Échec de la construction du projet
    exit /b 1
)

echo [SUCCESS] Processus de build terminé avec succès
endlocal
pause
