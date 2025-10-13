# Define colors for output
$Color_Off = "`e[0m"
$Red = "`e[0;31m"
$Green = "`e[0;32m"

# Function to print an error message and exit
function Error-Exit {
    param ([string]$Message)
    Write-Host -ForegroundColor Red $Message
    exit 1
}

# Check if Conan is installed
function Install-Conan {
    if (-not (Get-Command conan -ErrorAction SilentlyContinue)) {
        Write-Host "Conan is not installed. Installing Conan..."
        
        # Check if Python and pip are installed
        if (-not (Get-Command python -ErrorAction SilentlyContinue)) {
            Error-Exit "Python is not installed. Please install Python and try again."
        }
        
        # Attempt to install Conan using pip
        python -m pip install --upgrade pip
        python -m pip install conan
        if (-not (Get-Command conan -ErrorAction SilentlyContinue)) {
            Error-Exit "Failed to install Conan. Exiting..."
        }
    }
    else {
        Write-Host "${Green}Conan is already installed.${Color_Off}"
    }
}

# Configure Conan profile
function Configure-Conan {
    # Set up the default profile
    conan profile detect --force --name default | Out-Null
    if ($LASTEXITCODE -ne 0) {
        Error-Exit "Failed to detect Conan profile. Exiting..."
    }

    # Enable system requirements mode for Conan
    $env:CONAN_SYSREQUIRES_MODE = "enabled"
    Write-Host "${Green}Conan profile configured successfully.${Color_Off}"
}

# Install dependencies
function Install-Dependencies {
    # Create build directory if it doesn't exist
    if (!(Test-Path -Path "./build")) {
        New-Item -ItemType Directory -Path "./build" | Out-Null
    }
    Set-Location -Path "./build"
    
    # Run Conan install command
    conan install .. --build=missing -c tools.system.package_manager:mode=install -c tools.system.package_manager:sudo=True | Out-Null
    if ($LASTEXITCODE -ne 0) {
        Error-Exit "Conan install failed. Exiting..."
    }
    Write-Host "${Green}Dependencies installed successfully.${Color_Off}"
}

# Main script execution
Install-Conan
Configure-Conan
Install-Dependencies

Write-Host "${Green}Conan setup process completed successfully!${Color_Off}"
