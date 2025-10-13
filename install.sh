#!/bin/bash

# Colors for output
Color_Off='\033[0m'
Red='\033[0;31m'

OS_TYPE="UNKNOWN"

# Detect the OS type
if [[ "$OSTYPE" == "linux-gnu"* ]]; then
    OS_TYPE="Linux"
elif [[ "$OSTYPE" == "msys"* || "$OSTYPE" == "cygwin"* || "$OS" == "Windows_NT" ]]; then
    OS_TYPE="Windows"
else
    echo "Operating system not recognized. Exiting..."
    exit 1
fi

echo "Detected OS: $OS_TYPE"

# Remove existing build directory if it exists
if [ -d "build" ]; then
    echo "Removing existing build directory..."
    rm -rf build/
fi

if ! command -v conan &> /dev/null; then
    echo "Conan is not installed. Installing Conan..."
    if command -v pip3 &> /dev/null; then
        pip3 install conan || { echo "Failed to install Conan. Exiting..."; exit 1; }
    else
        echo "pip3 is not available. Please install pip3 first. Exiting..."
        exit 1
    fi
else
    echo "Conan is already installed."
fi

conan profile detect --force --name default || exit 1

export CONAN_SYSREQUIRES_MODE=enabled


mkdir build && cd build || exit 1


conan install .. --build=missing -c tools.system.package_manager:mode=install -c tools.system.package_manager:sudo=True || exit 1


if [ "$OS_TYPE" = "Linux" ]; then

    echo "Running full build process for Linux..."

    cmake .. -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release || exit 1

    cmake --build . || exit 1

    echo "Copying binaries for Linux..."
    cp ./r-type_client ../r-type_client && echo "BUILD: [Client binary created]" || exit 1
    cp ./r-type_server ../r-type_server && echo "BUILD: [Server binary created]" || exit 1
    #cp ./r-type_designer ../r-type_designer && echo "BUILD: [Designer binary created]" || exit 1

elif [ "$OS_TYPE" = "Windows" ]; then
    echo "Dependencies installed successfully on Windows. You can now run CMake manually for the build."
fi


echo -e "${Red}Process completed successfully!${Color_Off}"
