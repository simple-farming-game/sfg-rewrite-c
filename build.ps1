# Remove existing Release directory
if (Test-Path -Path .\build\Release) {
    Remove-Item -Path .\build\Release -Recurse -Force
    echo "[INFO] Successfully removed the existing Release directory."
} else {
    echo "[INFO] Release directory not found. Proceeding without deletion."
}

# Print separator
echo --------------------
echo build
echo --------------------

# Generate Release build configuration with CMake
cmake -Bbuild . -DCMAKE_BUILD_TYPE=Release -DCMAKE_INTERPROCEDURAL_OPTIMIZATION=ON

# Build Release configuration
cmake --build build --config Release --target SFG

# Print separator
echo --------------------
echo run
echo --------------------

# Navigate to directory and execute
.\build\Release\SFG.exe
